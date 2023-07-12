#include <CTextToCPP.h>

#include <algorithm>

namespace Codegenerator {

CTextToCPP::CTextToCPP() : next(nullptr) {}

CTextToCPP::~CTextToCPP() {
  clear();
  LOG(DEBUG) << "Destruktor: " << name << endl;
}

bool CTextToCPP::hasNext() { return next != nullptr; }

/**
 * @brief Generates the content of a header file
 *
 * @return string Content of the header file
 */
string CTextToCPP::writeDeclaration() { return next->writeDeclaration(); }

/**
 * @brief Generates the content of a source file
 *
 * @return string Content of the source file
 */
string CTextToCPP::writeImplementation() {
  // TODO: implement

  return "Implementation";
}

/**
 * @brief Adds an element to the concatenation
 *
 * @param element Element to add
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
 * @brief Sorts the elements
 *
 */
void CTextToCPP::sort() {
  // TODO: implement
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

}  // namespace Codegenerator