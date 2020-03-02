#ifndef CF_BLOCKING_READER_H
#define CF_BLOCKING_READER_H


#include <boost/asio.hpp>

// A simple blocking reader with timeout
class cf_blocking_reader {
public:

	// Constructs a blocking reader, pass in an open serial_port and
	// a timeout in milliseconds.
   cf_blocking_reader(boost::asio::io_service& ios, boost::asio::serial_port& port, size_t timeout);
   virtual ~cf_blocking_reader();

	// Reads a character or times out
	// returns false if the read times out
	bool read_char(char& val);

protected:

   // Called when an async read completes or has been cancelled
   void read_complete(const boost::system::error_code& error, size_t bytes_transferred);

   // Called when the timer's deadline expires.
   void time_out(const boost::system::error_code& error);

private:
   boost::asio::io_service&    m_ios;
	boost::asio::serial_port&   m_port;
	size_t                      m_timeout;
	char                        m_c;
	boost::asio::deadline_timer m_timer;

	size_t                      m_bytes_transferred;
};

#endif // CF_BLOCKING_READER_H
