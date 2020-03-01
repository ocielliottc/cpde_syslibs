#ifndef CF_SERIAL_SIMPLE_H
#define CF_SERIAL_SIMPLE_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

// An extremely simplified serial communication class
// Typical use is communication with Arduino
class cf_serial_simple {
public:
   typedef boost::asio::io_service    boost_io_service;
   typedef boost::asio::serial_port   boost_serial_port;

   // open serial port, keep it open for as long as object lives
   cf_serial_simple(const std::string& port, size_t baud_rate = 9600);
   virtual ~cf_serial_simple();

   // read string, assign to 'text'
   // returns true  if terminating character is found
   //         false if timeout occurred
   bool read_string(std::string& text, size_t timeout_ms, const char& term ='\n');

   // write string to serial port
   void write_string(const std::string& text);

protected:

   // read string from serial port
   std::string read_some();

private:
   boost_io_service                   m_ios;   // io service mus exist during communication
   std::shared_ptr<boost_serial_port> m_sp;    // the serial port
};

#endif // CF_SERIAL_SIMPLE_H
