#include "replace_substring.h"

void replace_substring(std::string& line, std::string& oldString, std::string& newString)
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

std::string replace_substring(const std::string& line, std::string& oldString, std::string& newString)
{
   std::string line_copy(line);
   replace_substring(line_copy,oldString,newString);
   return std::move(line_copy);
}
