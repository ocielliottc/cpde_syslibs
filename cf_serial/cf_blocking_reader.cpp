#include "cf_blocking_reader.h"
#include <boost/bind.hpp>

// https://www.ridgesolutions.ie/index.php/2012/12/13/boost-c-read-from-serial-port-with-timeout-example/

cf_blocking_reader::cf_blocking_reader(boost::asio::io_service& ios, boost::asio::serial_port& port, size_t timeout)
: m_ios(ios)
, m_port(port)
, m_timeout(timeout)
, m_timer(m_ios)
, m_bytes_transferred(0)
{}

cf_blocking_reader::~cf_blocking_reader()
{}

// Reads a character or times out
// returns false if the read times out
bool cf_blocking_reader::read_char(char& val)
{
   m_bytes_transferred = 0;

   val = m_c = '\0';

   // After a timeout & cancel it seems we need
   // to do a reset for subsequent reads to work.
   m_ios.reset();

   // Asynchronously read 1 character.
   boost::asio::async_read(m_port, boost::asio::buffer(&m_c, 1),
   boost::bind(&cf_blocking_reader::read_complete,this,boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

   // Setup a deadline time to implement our timeout.
   m_timer.expires_from_now(boost::posix_time::milliseconds(m_timeout));
   m_timer.async_wait(boost::bind(&cf_blocking_reader::time_out,this, boost::asio::placeholders::error));

   // This will block until a character is read
   // or until it is cancelled.
   m_ios.run();

   val = m_c;

   return (val != '\0');
}


// Called when an async read completes or has been cancelled
void cf_blocking_reader::read_complete(const boost::system::error_code& error, size_t bytes_transferred)
{
   m_bytes_transferred = bytes_transferred;

   // Read has finished, so cancel the timer.
   m_timer.cancel();
}

// Called when the timer's deadline expires.
void  cf_blocking_reader::time_out(const boost::system::error_code& error)
{
   // Was the timeout cancelled?
   if (error) {
      // yes
      return;
   }

   // no, we have timed out, so kill
   // the read operation
   // The read callback will be called
   // with an error
   m_port.cancel();
}
