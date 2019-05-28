#include "replace_substring.h"
#include <list>
#include <fstream>

void replace_substring(std::string& line, const std::string& oldString, const std::string& newString)
{
   const size_t oldSize = oldString.length();

   // do nothing if line is shorter than the string to find
   if( oldSize > line.length() ) return;

   const size_t newSize = newString.length();
   for( size_t pos = 0; ; pos += newSize ) {

      // Locate the substring to replace
      pos = line.find( oldString, pos );
      if( pos == std::string::npos ) return;

      if( oldSize == newSize ) {
         // if they're same size, use std::string::replace
         line.replace( pos, oldSize, newString );
      }
      else {
         // if not same size, replace by erasing and inserting
         line.erase( pos, oldSize );
         line.insert( pos, newString );
      }
   }
}

std::string replace_substring_copy(const std::string& line, const std::string& oldString, const std::string& newString)
{
   std::string line_copy(line);
   replace_substring(line_copy,oldString,newString);
   return std::move(line_copy);
}


void replace_substring_in_file(const std::string& file_path, const std::string& oldString, const std::string& newString)
{
   // perform in-place replacement

   // first read the file and store in list
   std::list<std::string> lines;
   {
      std::ifstream in(file_path);
      if(in.is_open()) {
         std::string line;
         while(std::getline(in,line)) {
            lines.push_back(replace_substring_copy(line,oldString,newString));
         }
      }
      else {
         throw std::runtime_error("File could not be opened for read: " + file_path);
      }
   }

   // write to same file
   {
      std::ofstream out(file_path);
      if(out.is_open()) {
         for(auto& line : lines) {
            out << line << std::endl;
         }
      }
      else {
         throw std::runtime_error("File could not be opened for write: " + file_path);
      }
   }
}
