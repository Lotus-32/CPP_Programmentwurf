#include <CTextToCPP.h>

#include <algorithm>

namespace Codegenerator {

CTextToCPP::CTextToCPP() : next(nullptr) {}

CTextToCPP::~CTextToCPP() { clear(); }

/**
 * @brief Generates the content of a header file
 *
 * @return string Content of the header file
 */
string CTextToCPP::writeDeclaration() {
  if (name == "") {
    return next->writeDeclaration();
  }
  string declaration;

  if (doxygen != "") {
    declaration += "/** " + doxygen;
    if (addtextpos) {
      declaration += " (aus Zeile " + to_string(addtextpos) + ")";
    }
    declaration += " */\n";
  }

  declaration += VAR_DECLARATION + name + ";\n";

  if (next != nullptr) {
    declaration += next->writeDeclaration();
  }
  return declaration;
}

/**
 * @brief Generates the content of a source file
 *
 * @return string Content of the source file
 */
string CTextToCPP::writeImplementation() { return next->writeImplementation(); }

/**
 * @brief Adds an element to the concatenation
 *
 * @param element The element to add
 */
void CTextToCPP::addElement(CTextToCPP* element) {
  // Falls next nicht null ist
  if (next != nullptr) {
    // Füge das Element dem nächsten Element hinzu
    next->addElement(element);
  } else {
    // Setze das nächste Element auf das übergebene Element
    next = element;
  }
}
/**
 * @brief Sorts the elements by name with insertion sort
 *
 */
void CTextToCPP::sort() {
  if (next == nullptr) {
    return;
  }

  CTextToCPP* sortedList = nullptr;
  CTextToCPP* current = next;

  while (current != nullptr) {
    CTextToCPP* nextElement = current->next;
    insert(sortedList, current);
    current = nextElement;
  }
  next = sortedList;
}

/**
 * The function inserts a new element into a sorted linked list based on the
 * element's name.
 *
 * @param sortedList A pointer to the head of a sorted linked list of CTextToCPP
 * objects.
 * @param newElement A pointer to the element that sould be inserted into the
 * list.
 */
void CTextToCPP::insert(CTextToCPP*& sortedList, CTextToCPP* newElement) {
  if (sortedList == nullptr || newElement->getName() < sortedList->getName()) {
    newElement->next = sortedList;
    sortedList = newElement;
  } else {
    CTextToCPP* current = sortedList;

    while (current->next != nullptr &&
           current->next->getName() < newElement->getName()) {
      current = current->next;
    }

    newElement->next = current->next;
    current->next = newElement;
  }
}

/**
 * @brief Clears the elements
 *
 */
void CTextToCPP::clear() {
  if (next != nullptr) {
    next->clear();
    delete next;
    next = nullptr;
  }
}

/**
 * The function `getName` returns the value of the `name` variable as a string.
 *
 * @return the value of the variable "name".
 */
string CTextToCPP::getName() const { return name; }

string CTextToCPP::getOriginalTextComment() {
  return "\n/*\nOriginaltext aus der Variablensektion '" + name + "'\n\n" +
         text + "\n*/\n";
}

string CTextToCPP::wordWrap(string text, char cut, bool isRaw,
                            int maxLineLength) {
  string wrappedText;
  int lineLength = 0;
  int lastSpace = 0;

  for (int i = 0; i < text.length(); i++) {
    if (text[i] == cut) {
      lastSpace = i;
    }

    if (lineLength >= maxLineLength) {
      if (isRaw) {
        wrappedText += text.substr(0, lastSpace) + " \\\n";
      } else {
        wrappedText += "\"" + text.substr(0, lastSpace) + "\" \\\n";
      }
      text = text.substr(lastSpace);
      lineLength = 0;
      i = 0;
    }

    lineLength++;
  }

  if (isRaw) {
    wrappedText += text;
  } else {
    wrappedText += "\"" + text + "\"";
  }

  return wrappedText;
}

}  // namespace Codegenerator