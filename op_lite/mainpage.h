#ifndef MAINPAGE_H
#define MAINPAGE_H

/*!
\mainpage op_lite - a portable, lightweight object persistence library for C++
Please note document generation date at bottom. 

\section license License
op_lite is developed by Carsten Arnholm, 2010-2016.

\code
 Author: Carsten Arnholm, January 2010-2016
 This code was written in my spare time for my weather station project

 This code follows the sqlite3 license model, i.e.:
 The author disclaims copyright to this source code.  In place of
 a legal notice, here is a blessing:

    May you do good and not evil.
    May you find forgiveness for yourself and forgive others.
    May you share freely, never taking more than you give.
\endcode


\subsection sub2 Disclaimer
<table>
<tr bgcolor="#ffffcc"> 
<td>
The author regularly uses the software on several of his computers and firmly believes that the software is without any damaging effect on anything, including hardware, data files or software. Nevertheless, the author accepts no responsibility for damages resulting from use of the software and makes no warranty or representation, either express or implied, including but not limited to, any implied warranty of merchantability or fitness for a particular purpose. The software is provided "AS IS" and the author does not assume or accept any risk by its use. By using this software you accept full responsibility for damages resulting from use of the software .
</td>
</tr>
</table>

\section intro Introduction

The name <EM>op_lite</EM> stands for <EM>Object Persistence – Lightweight</EM> . It is a C++ library that offers in-process object persistence of C++ objects, the application never explicitly reads or writes to the database – it all happens behind the scenes. From the <STRONG>op_lite white paper</STRONG> (available in PDF format): <EM>op_lite's objective is to provide an efficient, easy to use object oriented database, with support for persistent containers and polymorphic pointers.</EM>

op_lite has been developed for and tested under Linux Kubuntu (64 bit) using the GNU g++ compiler and under Windows 7 using the MS Visual C++ 2010 Express compiler (free).

op_lite is implemented using <a href="http://www.sqlite.org/">SQLite</a> as an efficient back-end, but takes a different approach to other libraries encapsulating SQLite. In op_lite, the goal is to provide a simple and natural object oriented API, somewhat comparable to commercial OODBMS systems in the sense that using persistent objects is largely transparent, read and write happens behind the scenes. The back end being used is therefore to be considered an implementation detail and mostly hidden from the application programmer:
// @image html op_lite.png 


An application programmer using op_lite implements persistent classes by inheriting from op_object:
// @image html op_lite_persistent_point.png 

For more information, please refer to the <STRONG>op_lite white paper</STRONG> (PDF) and the pages of this API documentation.

\subsection sub1 Dependencies

op_lite depends on  <b><a href="http://www.boost.org/">boost</a> </b>  and <b><a href="http://msgpack.org/">MessagePack</a> </b>.
op_lite includes source code from <b><a href="http://www.sqlite.org/">SQLite</a></b> , <b><a href="https://code.google.com/p/miniz/">miniz</a> </b> and selected parts of <b><a href="http://www.7-zip.org/sdk.html">LZMA SDK</a> </b>.

For more information on how to build op_lite, see <STRONG>op_lite white paper</STRONG> (PDF) and provided makefiles.

*/

#endif