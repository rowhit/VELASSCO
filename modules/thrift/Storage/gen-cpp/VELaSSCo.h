/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef VELaSSCo_H
#define VELaSSCo_H

#include <thrift/TDispatchProcessor.h>
#include "VELaSSCo_types.h"



class VELaSSCoIf {
 public:
  virtual ~VELaSSCoIf() {}

  /**
   * Return the status of the different services
   * which run on the Data Layer.
   */
  virtual void statusDL(std::string& _return) = 0;

  /**
   * Data Query operations
   * 
   * @param sessionID
   * @param model_group_qualifier
   * @param model_name_pattern
   */
  virtual void GetListOfModelNames(rvGetListOfModels& _return, const std::string& sessionID, const std::string& model_group_qualifier, const std::string& model_name_pattern) = 0;

  /**
   * Return the status of the different services
   * which run on the Data Layer.
   * @return string - returns a structured list of avialbe vertices,
   * with the attached list of double
   * if errors occur the contect is also returned here?
   * 
   * @param sessionID
   * @param modelID
   * @param analysisID
   * @param timeStep
   * @param resultID
   * @param listOfVertices
   */
  virtual void GetResultFromVerticesID(std::string& _return, const std::string& sessionID, const std::string& modelID, const std::string& analysisID, const double timeStep, const std::string& resultID, const std::string& listOfVertices) = 0;

  /**
   * Stop Data Layer
   */
  virtual void stopAll() = 0;
};

class VELaSSCoIfFactory {
 public:
  typedef VELaSSCoIf Handler;

  virtual ~VELaSSCoIfFactory() {}

  virtual VELaSSCoIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(VELaSSCoIf* /* handler */) = 0;
};

class VELaSSCoIfSingletonFactory : virtual public VELaSSCoIfFactory {
 public:
  VELaSSCoIfSingletonFactory(const boost::shared_ptr<VELaSSCoIf>& iface) : iface_(iface) {}
  virtual ~VELaSSCoIfSingletonFactory() {}

  virtual VELaSSCoIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(VELaSSCoIf* /* handler */) {}

 protected:
  boost::shared_ptr<VELaSSCoIf> iface_;
};

class VELaSSCoNull : virtual public VELaSSCoIf {
 public:
  virtual ~VELaSSCoNull() {}
  void statusDL(std::string& /* _return */) {
    return;
  }
  void GetListOfModelNames(rvGetListOfModels& /* _return */, const std::string& /* sessionID */, const std::string& /* model_group_qualifier */, const std::string& /* model_name_pattern */) {
    return;
  }
  void GetResultFromVerticesID(std::string& /* _return */, const std::string& /* sessionID */, const std::string& /* modelID */, const std::string& /* analysisID */, const double /* timeStep */, const std::string& /* resultID */, const std::string& /* listOfVertices */) {
    return;
  }
  void stopAll() {
    return;
  }
};


class VELaSSCo_statusDL_args {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  VELaSSCo_statusDL_args(const VELaSSCo_statusDL_args&);
  VELaSSCo_statusDL_args& operator=(const VELaSSCo_statusDL_args&);
  VELaSSCo_statusDL_args() {
  }

  virtual ~VELaSSCo_statusDL_args() throw();

  bool operator == (const VELaSSCo_statusDL_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const VELaSSCo_statusDL_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_statusDL_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_statusDL_args& obj);
};


class VELaSSCo_statusDL_pargs {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~VELaSSCo_statusDL_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_statusDL_pargs& obj);
};

typedef struct _VELaSSCo_statusDL_result__isset {
  _VELaSSCo_statusDL_result__isset() : success(false) {}
  bool success :1;
} _VELaSSCo_statusDL_result__isset;

class VELaSSCo_statusDL_result {
 public:

  static const char* ascii_fingerprint; // = "9A73381FEFD6B67F432E717102246330";
  static const uint8_t binary_fingerprint[16]; // = {0x9A,0x73,0x38,0x1F,0xEF,0xD6,0xB6,0x7F,0x43,0x2E,0x71,0x71,0x02,0x24,0x63,0x30};

  VELaSSCo_statusDL_result(const VELaSSCo_statusDL_result&);
  VELaSSCo_statusDL_result& operator=(const VELaSSCo_statusDL_result&);
  VELaSSCo_statusDL_result() : success() {
  }

  virtual ~VELaSSCo_statusDL_result() throw();
  std::string success;

  _VELaSSCo_statusDL_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const VELaSSCo_statusDL_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const VELaSSCo_statusDL_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_statusDL_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_statusDL_result& obj);
};

typedef struct _VELaSSCo_statusDL_presult__isset {
  _VELaSSCo_statusDL_presult__isset() : success(false) {}
  bool success :1;
} _VELaSSCo_statusDL_presult__isset;

class VELaSSCo_statusDL_presult {
 public:

  static const char* ascii_fingerprint; // = "9A73381FEFD6B67F432E717102246330";
  static const uint8_t binary_fingerprint[16]; // = {0x9A,0x73,0x38,0x1F,0xEF,0xD6,0xB6,0x7F,0x43,0x2E,0x71,0x71,0x02,0x24,0x63,0x30};


  virtual ~VELaSSCo_statusDL_presult() throw();
  std::string* success;

  _VELaSSCo_statusDL_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_statusDL_presult& obj);
};

typedef struct _VELaSSCo_GetListOfModelNames_args__isset {
  _VELaSSCo_GetListOfModelNames_args__isset() : sessionID(false), model_group_qualifier(false), model_name_pattern(false) {}
  bool sessionID :1;
  bool model_group_qualifier :1;
  bool model_name_pattern :1;
} _VELaSSCo_GetListOfModelNames_args__isset;

class VELaSSCo_GetListOfModelNames_args {
 public:

  static const char* ascii_fingerprint; // = "AB879940BD15B6B25691265F7384B271";
  static const uint8_t binary_fingerprint[16]; // = {0xAB,0x87,0x99,0x40,0xBD,0x15,0xB6,0xB2,0x56,0x91,0x26,0x5F,0x73,0x84,0xB2,0x71};

  VELaSSCo_GetListOfModelNames_args(const VELaSSCo_GetListOfModelNames_args&);
  VELaSSCo_GetListOfModelNames_args& operator=(const VELaSSCo_GetListOfModelNames_args&);
  VELaSSCo_GetListOfModelNames_args() : sessionID(), model_group_qualifier(), model_name_pattern() {
  }

  virtual ~VELaSSCo_GetListOfModelNames_args() throw();
  std::string sessionID;
  std::string model_group_qualifier;
  std::string model_name_pattern;

  _VELaSSCo_GetListOfModelNames_args__isset __isset;

  void __set_sessionID(const std::string& val);

  void __set_model_group_qualifier(const std::string& val);

  void __set_model_name_pattern(const std::string& val);

  bool operator == (const VELaSSCo_GetListOfModelNames_args & rhs) const
  {
    if (!(sessionID == rhs.sessionID))
      return false;
    if (!(model_group_qualifier == rhs.model_group_qualifier))
      return false;
    if (!(model_name_pattern == rhs.model_name_pattern))
      return false;
    return true;
  }
  bool operator != (const VELaSSCo_GetListOfModelNames_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_GetListOfModelNames_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetListOfModelNames_args& obj);
};


class VELaSSCo_GetListOfModelNames_pargs {
 public:

  static const char* ascii_fingerprint; // = "AB879940BD15B6B25691265F7384B271";
  static const uint8_t binary_fingerprint[16]; // = {0xAB,0x87,0x99,0x40,0xBD,0x15,0xB6,0xB2,0x56,0x91,0x26,0x5F,0x73,0x84,0xB2,0x71};


  virtual ~VELaSSCo_GetListOfModelNames_pargs() throw();
  const std::string* sessionID;
  const std::string* model_group_qualifier;
  const std::string* model_name_pattern;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetListOfModelNames_pargs& obj);
};

typedef struct _VELaSSCo_GetListOfModelNames_result__isset {
  _VELaSSCo_GetListOfModelNames_result__isset() : success(false) {}
  bool success :1;
} _VELaSSCo_GetListOfModelNames_result__isset;

class VELaSSCo_GetListOfModelNames_result {
 public:

  static const char* ascii_fingerprint; // = "9FF622309718E2479AE3E37AADE5C533";
  static const uint8_t binary_fingerprint[16]; // = {0x9F,0xF6,0x22,0x30,0x97,0x18,0xE2,0x47,0x9A,0xE3,0xE3,0x7A,0xAD,0xE5,0xC5,0x33};

  VELaSSCo_GetListOfModelNames_result(const VELaSSCo_GetListOfModelNames_result&);
  VELaSSCo_GetListOfModelNames_result& operator=(const VELaSSCo_GetListOfModelNames_result&);
  VELaSSCo_GetListOfModelNames_result() {
  }

  virtual ~VELaSSCo_GetListOfModelNames_result() throw();
  rvGetListOfModels success;

  _VELaSSCo_GetListOfModelNames_result__isset __isset;

  void __set_success(const rvGetListOfModels& val);

  bool operator == (const VELaSSCo_GetListOfModelNames_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const VELaSSCo_GetListOfModelNames_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_GetListOfModelNames_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetListOfModelNames_result& obj);
};

typedef struct _VELaSSCo_GetListOfModelNames_presult__isset {
  _VELaSSCo_GetListOfModelNames_presult__isset() : success(false) {}
  bool success :1;
} _VELaSSCo_GetListOfModelNames_presult__isset;

class VELaSSCo_GetListOfModelNames_presult {
 public:

  static const char* ascii_fingerprint; // = "9FF622309718E2479AE3E37AADE5C533";
  static const uint8_t binary_fingerprint[16]; // = {0x9F,0xF6,0x22,0x30,0x97,0x18,0xE2,0x47,0x9A,0xE3,0xE3,0x7A,0xAD,0xE5,0xC5,0x33};


  virtual ~VELaSSCo_GetListOfModelNames_presult() throw();
  rvGetListOfModels* success;

  _VELaSSCo_GetListOfModelNames_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetListOfModelNames_presult& obj);
};

typedef struct _VELaSSCo_GetResultFromVerticesID_args__isset {
  _VELaSSCo_GetResultFromVerticesID_args__isset() : sessionID(false), modelID(false), analysisID(false), timeStep(false), resultID(false), listOfVertices(false) {}
  bool sessionID :1;
  bool modelID :1;
  bool analysisID :1;
  bool timeStep :1;
  bool resultID :1;
  bool listOfVertices :1;
} _VELaSSCo_GetResultFromVerticesID_args__isset;

class VELaSSCo_GetResultFromVerticesID_args {
 public:

  static const char* ascii_fingerprint; // = "028F20434CE844656C7FFE290B942938";
  static const uint8_t binary_fingerprint[16]; // = {0x02,0x8F,0x20,0x43,0x4C,0xE8,0x44,0x65,0x6C,0x7F,0xFE,0x29,0x0B,0x94,0x29,0x38};

  VELaSSCo_GetResultFromVerticesID_args(const VELaSSCo_GetResultFromVerticesID_args&);
  VELaSSCo_GetResultFromVerticesID_args& operator=(const VELaSSCo_GetResultFromVerticesID_args&);
  VELaSSCo_GetResultFromVerticesID_args() : sessionID(), modelID(), analysisID(), timeStep(0), resultID(), listOfVertices() {
  }

  virtual ~VELaSSCo_GetResultFromVerticesID_args() throw();
  std::string sessionID;
  std::string modelID;
  std::string analysisID;
  double timeStep;
  std::string resultID;
  std::string listOfVertices;

  _VELaSSCo_GetResultFromVerticesID_args__isset __isset;

  void __set_sessionID(const std::string& val);

  void __set_modelID(const std::string& val);

  void __set_analysisID(const std::string& val);

  void __set_timeStep(const double val);

  void __set_resultID(const std::string& val);

  void __set_listOfVertices(const std::string& val);

  bool operator == (const VELaSSCo_GetResultFromVerticesID_args & rhs) const
  {
    if (!(sessionID == rhs.sessionID))
      return false;
    if (!(modelID == rhs.modelID))
      return false;
    if (!(analysisID == rhs.analysisID))
      return false;
    if (!(timeStep == rhs.timeStep))
      return false;
    if (!(resultID == rhs.resultID))
      return false;
    if (!(listOfVertices == rhs.listOfVertices))
      return false;
    return true;
  }
  bool operator != (const VELaSSCo_GetResultFromVerticesID_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_GetResultFromVerticesID_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetResultFromVerticesID_args& obj);
};


class VELaSSCo_GetResultFromVerticesID_pargs {
 public:

  static const char* ascii_fingerprint; // = "028F20434CE844656C7FFE290B942938";
  static const uint8_t binary_fingerprint[16]; // = {0x02,0x8F,0x20,0x43,0x4C,0xE8,0x44,0x65,0x6C,0x7F,0xFE,0x29,0x0B,0x94,0x29,0x38};


  virtual ~VELaSSCo_GetResultFromVerticesID_pargs() throw();
  const std::string* sessionID;
  const std::string* modelID;
  const std::string* analysisID;
  const double* timeStep;
  const std::string* resultID;
  const std::string* listOfVertices;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetResultFromVerticesID_pargs& obj);
};

typedef struct _VELaSSCo_GetResultFromVerticesID_result__isset {
  _VELaSSCo_GetResultFromVerticesID_result__isset() : success(false) {}
  bool success :1;
} _VELaSSCo_GetResultFromVerticesID_result__isset;

class VELaSSCo_GetResultFromVerticesID_result {
 public:

  static const char* ascii_fingerprint; // = "9A73381FEFD6B67F432E717102246330";
  static const uint8_t binary_fingerprint[16]; // = {0x9A,0x73,0x38,0x1F,0xEF,0xD6,0xB6,0x7F,0x43,0x2E,0x71,0x71,0x02,0x24,0x63,0x30};

  VELaSSCo_GetResultFromVerticesID_result(const VELaSSCo_GetResultFromVerticesID_result&);
  VELaSSCo_GetResultFromVerticesID_result& operator=(const VELaSSCo_GetResultFromVerticesID_result&);
  VELaSSCo_GetResultFromVerticesID_result() : success() {
  }

  virtual ~VELaSSCo_GetResultFromVerticesID_result() throw();
  std::string success;

  _VELaSSCo_GetResultFromVerticesID_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const VELaSSCo_GetResultFromVerticesID_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const VELaSSCo_GetResultFromVerticesID_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_GetResultFromVerticesID_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetResultFromVerticesID_result& obj);
};

typedef struct _VELaSSCo_GetResultFromVerticesID_presult__isset {
  _VELaSSCo_GetResultFromVerticesID_presult__isset() : success(false) {}
  bool success :1;
} _VELaSSCo_GetResultFromVerticesID_presult__isset;

class VELaSSCo_GetResultFromVerticesID_presult {
 public:

  static const char* ascii_fingerprint; // = "9A73381FEFD6B67F432E717102246330";
  static const uint8_t binary_fingerprint[16]; // = {0x9A,0x73,0x38,0x1F,0xEF,0xD6,0xB6,0x7F,0x43,0x2E,0x71,0x71,0x02,0x24,0x63,0x30};


  virtual ~VELaSSCo_GetResultFromVerticesID_presult() throw();
  std::string* success;

  _VELaSSCo_GetResultFromVerticesID_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_GetResultFromVerticesID_presult& obj);
};


class VELaSSCo_stopAll_args {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  VELaSSCo_stopAll_args(const VELaSSCo_stopAll_args&);
  VELaSSCo_stopAll_args& operator=(const VELaSSCo_stopAll_args&);
  VELaSSCo_stopAll_args() {
  }

  virtual ~VELaSSCo_stopAll_args() throw();

  bool operator == (const VELaSSCo_stopAll_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const VELaSSCo_stopAll_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_stopAll_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_stopAll_args& obj);
};


class VELaSSCo_stopAll_pargs {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~VELaSSCo_stopAll_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_stopAll_pargs& obj);
};


class VELaSSCo_stopAll_result {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  VELaSSCo_stopAll_result(const VELaSSCo_stopAll_result&);
  VELaSSCo_stopAll_result& operator=(const VELaSSCo_stopAll_result&);
  VELaSSCo_stopAll_result() {
  }

  virtual ~VELaSSCo_stopAll_result() throw();

  bool operator == (const VELaSSCo_stopAll_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const VELaSSCo_stopAll_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VELaSSCo_stopAll_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_stopAll_result& obj);
};


class VELaSSCo_stopAll_presult {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};


  virtual ~VELaSSCo_stopAll_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

  friend std::ostream& operator<<(std::ostream& out, const VELaSSCo_stopAll_presult& obj);
};

class VELaSSCoClient : virtual public VELaSSCoIf {
 public:
  VELaSSCoClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  VELaSSCoClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void statusDL(std::string& _return);
  void send_statusDL();
  void recv_statusDL(std::string& _return);
  void GetListOfModelNames(rvGetListOfModels& _return, const std::string& sessionID, const std::string& model_group_qualifier, const std::string& model_name_pattern);
  void send_GetListOfModelNames(const std::string& sessionID, const std::string& model_group_qualifier, const std::string& model_name_pattern);
  void recv_GetListOfModelNames(rvGetListOfModels& _return);
  void GetResultFromVerticesID(std::string& _return, const std::string& sessionID, const std::string& modelID, const std::string& analysisID, const double timeStep, const std::string& resultID, const std::string& listOfVertices);
  void send_GetResultFromVerticesID(const std::string& sessionID, const std::string& modelID, const std::string& analysisID, const double timeStep, const std::string& resultID, const std::string& listOfVertices);
  void recv_GetResultFromVerticesID(std::string& _return);
  void stopAll();
  void send_stopAll();
  void recv_stopAll();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class VELaSSCoProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<VELaSSCoIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (VELaSSCoProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_statusDL(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetListOfModelNames(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetResultFromVerticesID(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_stopAll(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  VELaSSCoProcessor(boost::shared_ptr<VELaSSCoIf> iface) :
    iface_(iface) {
    processMap_["statusDL"] = &VELaSSCoProcessor::process_statusDL;
    processMap_["GetListOfModelNames"] = &VELaSSCoProcessor::process_GetListOfModelNames;
    processMap_["GetResultFromVerticesID"] = &VELaSSCoProcessor::process_GetResultFromVerticesID;
    processMap_["stopAll"] = &VELaSSCoProcessor::process_stopAll;
  }

  virtual ~VELaSSCoProcessor() {}
};

class VELaSSCoProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  VELaSSCoProcessorFactory(const ::boost::shared_ptr< VELaSSCoIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< VELaSSCoIfFactory > handlerFactory_;
};

class VELaSSCoMultiface : virtual public VELaSSCoIf {
 public:
  VELaSSCoMultiface(std::vector<boost::shared_ptr<VELaSSCoIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~VELaSSCoMultiface() {}
 protected:
  std::vector<boost::shared_ptr<VELaSSCoIf> > ifaces_;
  VELaSSCoMultiface() {}
  void add(boost::shared_ptr<VELaSSCoIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void statusDL(std::string& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->statusDL(_return);
    }
    ifaces_[i]->statusDL(_return);
    return;
  }

  void GetListOfModelNames(rvGetListOfModels& _return, const std::string& sessionID, const std::string& model_group_qualifier, const std::string& model_name_pattern) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetListOfModelNames(_return, sessionID, model_group_qualifier, model_name_pattern);
    }
    ifaces_[i]->GetListOfModelNames(_return, sessionID, model_group_qualifier, model_name_pattern);
    return;
  }

  void GetResultFromVerticesID(std::string& _return, const std::string& sessionID, const std::string& modelID, const std::string& analysisID, const double timeStep, const std::string& resultID, const std::string& listOfVertices) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetResultFromVerticesID(_return, sessionID, modelID, analysisID, timeStep, resultID, listOfVertices);
    }
    ifaces_[i]->GetResultFromVerticesID(_return, sessionID, modelID, analysisID, timeStep, resultID, listOfVertices);
    return;
  }

  void stopAll() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->stopAll();
    }
    ifaces_[i]->stopAll();
  }

};



#endif
