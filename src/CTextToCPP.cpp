#include <CTextToCPP.h>

#include <algorithm>

CTextToCPP::CTextToCPP() {}

CTextToCPP::~CTextToCPP() {}

/**
 * @brief Generates the content of a header file
 *
 * @return string Content of the header file
 */
string CTextToCPP::writeDeclaration() {
  // TODO: implement
  string result = "";
  for (string element : elements) {
    result += element + "\n";
  }
  return result;
}

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
void CTextToCPP::addElement(const string& element) {
  elements.push_back(element);
}

/**
 * @brief Sorts the elements
 *
 */
void CTextToCPP::sort() { std::sort(elements.begin(), elements.end()); }

/**
 * @brief Clears the elements
 *
 */
void CTextToCPP::clear() {
  elements.clear();
  // TODO: Check if more memory management is needed
}