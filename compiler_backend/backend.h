#pragma once

#include <string>
#include <memory>

/**
 * Generates assembly from an AST text file
 * @param astFile Path to the .ast file (text representation of AST)
 * @param outputFile Path where to save the generated .asm file
 * @return true on success, false on error
 */
bool generateAssembly(const std::string& astFile, 
                      const std::string& outputFile);
