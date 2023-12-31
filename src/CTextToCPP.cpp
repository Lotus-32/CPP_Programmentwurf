/**
 * @file CTextToCPP.cpp
 * @author Marc Sachße
 * @brief Base class for converting text to C or C++ code
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <CTextToCPP.h>

#include <algorithm>

namespace Codegenerator {

CTextToCPP::CTextToCPP() : next(nullptr) {}

CTextToCPP::~CTextToCPP() { clear(); }

/**
 * The function `writeDeclaration` generates a C++ declaration for a variable,
 * including any associated doxygen comments.
 *
 * @return a string that represents the declaration of a variable.
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
  if (next != nullptr) {
    next->addElement(element);
  } else {
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
 * The function is a helper funciton for sort(). It inserts a new element into a
 * sorted linked list based on the element's name.
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

/**
 * The function returns a comment containing the original text from a variable
 * section and a introduction.
 *
 * @return a string that contains a comment block. The comment block includes
 * the original text from the variable section, which is obtained by
 * concatenating the name and text variables.
 */
string CTextToCPP::getOriginalTextComment() {
  return "\n/*\nOriginaltext aus der Variablensektion '" + name + "'\n\n" +
         text + "\n*/\n";
}

/**
 * The virtual function `wordWrap` takes a string `text`, a character `cut`, and
 * an integer `maxLineLength` as input, and returns a new string with the text
 * wrapped at the specified line length, using the specified character as the
 * cut point.
 *
 * @param text The `text` parameter is the input string that needs to be wrapped
 * into multiple lines.
 * @param cut The "cut" parameter is a character that is used to determine where
 * to wrap the text. When the "cut" character is encountered, the text will be
 * wrapped at that point.
 * @param maxLineLength The `maxLineLength` parameter specifies the maximum
 * length of each line in the wrapped text.
 *
 * @return the wrapped text, which is a string.
 */
string CTextToCPP::wordWrap(string text, const char cut, int maxLineLength) {
  string wrappedText;
  int lineLength = 0;
  int lastSpace = 0;

  for (int i = 0; i < text.length(); i++) {
    if (text[i] == cut) {
      lastSpace = i;
    }
    if (text[i] == '\n') {
      wrappedText += "\"" + text.substr(0, i) + "\" \\\n";
      text = text.substr(i + 1);
      lineLength = 0;
      i = 0;
      continue;
    }
    if (lineLength >= maxLineLength) {
      wrappedText += "\"" + text.substr(0, lastSpace) + "\" \\\n";
      text = text.substr(lastSpace);
      lineLength = 0;
      i = 0;
    }

    lineLength++;
  }

  return wrappedText + "\"" + text + "\"";
}

}  // namespace Codegenerator