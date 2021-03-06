/* -*- c++ -*- */
#pragma once

// CLib
#include <cctype>

// STL
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

// Boost
#include <boost/regex.hpp>

#include <malloc.h>
#include <string.h>
#include <ctype.h>

#include "Crono.h"

// ---------------------------------------------------------------------------

// seems that this is not working:
// #ifndef FUNCTION_NAME
// #   define FUNCTION_NAME ""
// #   ifdef __PRETTY_FUNCTION__
// #		undef FUNCTION_NAME
// #		define FUNCTION_NAME __PRETTY_FUNCTION__
// #	endif
// #	ifdef __FUNCTION__
// #		undef FUNCTION_NAME
// #		define FUNCTION_NAME __FUNCTION__
// #	endif
// #	ifdef __func__
// #		undef FUNCTION_NAME
// #		define FUNCTION_NAME __func__
// #	endif
// #endif

#ifdef WIN32
#  define FUNCTION_NAME  __FUNCSIG__
#else
#  define FUNCTION_NAME  __PRETTY_FUNCTION__
#endif

#define ENABLE_LOGGING
#ifdef ENABLE_LOGGING
// to know that the messages belongs to the EngineLayer
#	define LOGGER (std::cerr << "[VELaSSCo-EL] " << getStrCurrentWallTime() << " ")
#else
#	define LOGGER (std::ostream(0))
#endif
#define DEBUG( a)   LOGGER << a << std::endl;

#define PING					\
  do						\
    {						\
      LOGGER << __FUNCTION__ << std::endl;	\
    }						\
  while (0)

//      LOGGER << FUNCTION_NAME << std::endl;

// ---------------------------------------------------------------------------

// not using namespace in DataLayer at the moment
// namespace VELaSSCo
// {

  /**
   * Converts a std::string containing binary to a hex dump ASCII string.
   */ 

  inline std::string Hexdump(const std::string input, const size_t max_len = 80)
  {
    std::stringstream out;

    size_t end = input.size();
    if ( max_len && ( end > max_len)) {
      end = max_len;
    }
    for (size_t i=0; i<max_len; i+=16)
      {
	out << std::hex << std::setw(2*sizeof(size_t)) << std::setfill('0') << (size_t)i << ": ";
	for (size_t j=0; j<16; j++) {
	  if (i+j < input.size()) {
	    out << std::setw(2) << (unsigned)(unsigned char)(input[i+j]) << " ";
	  } else {
	    out << "   ";
	  }
	}

	out << " ";
	for (size_t j=0; j<16; j++) {
	  if (i+j < input.size()) {
	    if (isprint((unsigned char)input[i+j])) {
	      out << input[i+j];
	    } else {
	      out << '.';
	    }
	  }
	}

	out << std::endl;
      }

    return out.str();
  }
  
  /**
   * Dumps std::string. It is used to cut the long std::string in a specific @max_len.
   */ 

  inline std::string Strdump(const std::string input, const size_t max_len = 80)
  {
	std::string output;
    if(input.size() > max_len){
		output = std::string(input.begin(), input.begin() + max_len);
		output += "...\n";
	} else {
		output = input;
	}
	
	return output;
  }


	template<typename T>
	inline T byteSwap(T value)
	{
		if (sizeof(T) == 8)
		{
			uint64_t src = reinterpret_cast<uint64_t&>(value);
			uint64_t dst = __builtin_bswap64(src);
		
			return reinterpret_cast<T&>(dst);
		}
		else if (sizeof(T) == 4)
		{
			uint32_t src = reinterpret_cast<uint32_t&>(value);
			uint32_t dst = __builtin_bswap32(src);
		
			return reinterpret_cast<T&>(dst);
		}
	}
  template< typename T>
  inline T byteSwapIfNeed( T value, T saneLimit) {
    // swapping depends if value has been injected as big (from java) or little (from c++) endian ...
    return ( value > saneLimit) ? byteSwap< T>( value) : value;
  }
  template< typename T>
  inline T byteSwapIfNeedPositive( T value, T saneLimit) {
    // swapping depends if value has been injected as big (from java) or little (from c++) endian ...
    return ( ( value < 0) || ( value > saneLimit)) ? byteSwap< T>( value) : value;
  }

// this limits the highest ID to 1.000.000 billion
#define MAXIMUM_ALLOWED_I64_NUMBER ( ( int64_t)1024 * 1024 * 1024 * 1024 * 1024 * 1024)
// this limits the highest ID to 1 billion
#define MAXIMUM_ALLOWED_I32_NUMBER ( ( int)1024 * 1024 * 1024)


  // returns NULL if dst_len is too short, otherwise return dst
  inline const char *ToHexString( char *dst, size_t dst_len, const char *src, const size_t src_len, const char *format="%02x") {
    if ( !dst) return NULL;
    if ( dst_len <= 0) return NULL;
    size_t isrc = 0;
    for ( size_t idst = 0; 
	  ( isrc < src_len) && ( idst < dst_len - 1); 
	  isrc++, idst += 2) {
      sprintf( &dst[ idst], format, ( unsigned char)src[ isrc]);
    }
    // if all chars converted, then return dst
    return ( isrc == src_len) ? dst : NULL;
  }
  // returns NULL if dst_len is too short or error in conversion, otherwise return dst
  inline const char *FromHexString( char *dst, size_t dst_len, const char *src, const size_t src_len) {
    if ( !dst) return NULL;
    bool error = false;
    size_t isrc = 0;
    for ( size_t idst = 0; 
	  ( isrc < src_len) && ( idst < dst_len); 
	  isrc += 2, idst++) {
      int hi4 = tolower( ( int)( unsigned char)src[ isrc]);
      int lo4 = tolower( ( int)( unsigned char)src[ isrc + 1]);
      if ( isxdigit( hi4) && isxdigit( lo4)) {
	hi4 -= isdigit( hi4) ? '0' : ( 'a' - 10);
	lo4 -= isdigit( lo4) ? '0' : ( 'a' - 10);
	dst[ idst] = ( char)( unsigned char)( ( ( hi4 << 4) + lo4) & 0xff);
      } else {
	error = true;
	break;
      }
    }
    // if all chars converted and no error in conversion then return dst
    return ( ( isrc == src_len) && !error) ? dst : NULL;
  }

  inline std::string ModelID_DoHexStringConversionIfNecesary( const std::string &modelID, char *tmp_buf, const size_t size_tmp_buf) {
    if ( modelID.length() == 16) {
      return ( std::string) ToHexString( tmp_buf, size_tmp_buf, modelID.c_str(), modelID.size());
    } else {
      return modelID;
    }
  }

  /**
   * Compare two strings ignoring case (OS portable)
   */
  inline bool AreEqualNoCase( const std::string &a, const std::string &b) {
    if ( a.size() != b.size()) {
      return false;
    }
    for ( std::string::const_iterator c1 = a.begin(), c2 = b.begin(); c1 != a.end(); ++c1, ++c2) {
      if ( tolower( ( int)( unsigned char)*c1) != tolower( ( int)( unsigned char)*c2)) {
	return false;
      }
    }
    return true;
  }
  inline bool AreEqualNoCaseSubstr( const std::string &a, const std::string &b, const size_t len) {
    return AreEqualNoCase( a.substr( 0, len), b.substr( 0, len));
  }

  inline std::string ToLower( const std::string &str) {
    // something like:
    // std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    // may not work for "negative" chars
    std::string ret( "");
    for ( std::string::const_iterator c = str.begin(); c != str.end(); ++c) {
      ret += tolower( ( int)( unsigned char)*c);
    }
    return ret;
  }

  inline std::string ToUpper( const std::string &str) {
    std::string ret( "");
    for ( std::string::const_iterator c = str.begin(); c != str.end(); ++c) {
      ret += toupper( ( int)( unsigned char)*c);
    }
    return ret;
  }

inline std::string GetNiceSizeString( const size_t num_bytes, bool append_original_size = true) {
   // Human readable form:
   const char *units[] = { "Bytes", "KBytes", "MBytes", "GBytes", "TBytes", NULL};
   int idx = 0;
   double scaled_size = ( double)num_bytes;
   // make printable number between 0...1024 and have a valid unit suffix
   while ( ( scaled_size > 1024.0) && units[ idx + 1]) {
     scaled_size /= 1024.0;
     idx++;
   }
   std::ostringstream oss;
   oss << scaled_size << " " << units[ idx];
   if ( append_original_size) {
     oss << " ( " << num_bytes << " Bytes)";
   }
   return oss.str();
}

#ifdef _WIN32

#ifndef strcasecmp
#define strcasecmp  _stricmp
#endif

#endif /* _WIN32 */

  // ---------------------------------------------------------------------------
	
  /**
   * Separates a URL string into substrings like host, port, etc.
   */

  class URL
  {
  public:

    URL(std::string url) : m_url(url) 
    {
      const std::string URL_FORMAT = "((?<scheme>[a-zA-Z][a-zA-Z0-9+.-]*)://)?((?<userinfo>[^@]*)@)?(?<host>[a-zA-Z0-9.-]*)(:(?<port>[\\d]{1,5}))?(?<path>[/\\\\][^?#]*)?(\\?(?<query>[^#]*))?(#(?<fragment>.*))?";

      boost::regex expression(URL_FORMAT, boost::regex::icase);
      boost::smatch matches;
		
      if ( boost::regex_match(m_url, matches, expression) )
	{
	  m_scheme   = matches["scheme"];
	  m_userinfo = matches["userinfo"];
	  m_host     = matches["host"];
	  m_port     = matches["port"];
	  m_part     = matches["part"];
	  m_query    = matches["query"];
	  m_fragment = matches["fragment"];
	}
    };

    std::string Scheme()   { return m_scheme;   }
    std::string Userinfo() { return m_userinfo; }
    std::string Host()     { return m_host;     }
    std::string Port()     { return m_port;     }
    std::string Part()     { return m_part;     }
    std::string Query()    { return m_query;    }
    std::string Fragment() { return m_fragment; }

  private:
	
    std::string m_url;

    std::string m_scheme;
    std::string m_userinfo;
    std::string m_host;
    std::string m_port;
    std::string m_part;
    std::string m_query;
    std::string m_fragment;
  };

  // not using namespace in DataLayer at the moment
// } // namespace
