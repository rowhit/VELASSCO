
// ***
// *** Whithin the VELaSSCo AccessLib this server is only for testing purposes.
// ***

#include <assert.h>

// STL
#include <sstream>

// Thrift
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

// Boost
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

// VELaSSCo
// AccessLib.h is only used for the return codes ( enum values)
#include "../../AccessLib/AccessLib/AccessLib.h"
#include "Helpers.h"

// Generated code
#include "../../thrift/QueryManager/gen-cpp/QueryManager.h"

#include "DataLayerAccess.h"
#include "Analytics.h"
#include "Graphics.h"
#include "base64.h"

#include "Server.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

using namespace  ::VELaSSCo;

/* 1xx - Direct Result Queries */

/* first queries */

void QueryManagerServer::ManageGetResultFromVerticesID( Query_Result &_return, const SessionID sessionID, const std::string& query) {
  // Parse query JSON
  std::istringstream ss(query);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);

  std::string name       = pt.get<std::string>("name");
  std::string modelID    = pt.get<std::string>("modelID");
  std::string resultID   = pt.get<std::string>("resultID");
  std::string analysisID = pt.get<std::string>("analysisID");
  double      timeStep   = pt.get<double>("timeStep");
  
  //std::cout << "Vertex IDs = " << vertexIDs << std::endl;
  
  // the vertexID list is encoded in a base64 string
  // std::string vertexIDs  = as_string<int64_t>( pt, "vertexIDs");
  // std::vector<int64_t> listOfVertices;
  // std::string tmp_buf;
  // for(size_t i = 0; i < vertexIDs.size(); i++){
  // 	  if(vertexIDs[i] == '[') continue;
  // 	  else if(vertexIDs[i]==','){
  // 		  istringstream tmp_buf_stream(tmp_buf);
  // 		  int64_t vertexID;
  // 		  tmp_buf_stream >> vertexID;
  // 		  listOfVertices.push_back(vertexID);
  // 		  tmp_buf = "";
  // 	  } else if(vertexIDs[i] == ']'){
  // 		  istringstream tmp_buf_stream(tmp_buf);
  // 		  int64_t vertexID;
  // 		  tmp_buf_stream >> vertexID;
  // 		  listOfVertices.push_back(vertexID);
  // 		  tmp_buf = "";
  // 		  break;
  // 	  } else {
  // 		  tmp_buf += vertexIDs[i];
  // 	  }
  // }
  std::string vertexIDs  = pt.get<std::string>("vertexIDs");
  std::string raw_data = base64_decode( vertexIDs);
  size_t num_vertexIDs = raw_data.length() / sizeof( int64_t);
  int64_t *lst_vertexIDs = ( int64_t *)raw_data.data();
  std::vector<int64_t> listOfVertices( lst_vertexIDs, lst_vertexIDs + num_vertexIDs);
  // for ( size_t idx = 0; idx < num_vertexIDs; idx++) {
  //   listOfVertices.push_back( lst_vertexIDs[ idx]);
  // }

  bool decoded = false;
  if ( raw_data.length()) {
    decoded = true;
  } else {
    // old fashioned way:
    vertexIDs  = as_string<int64_t>( pt, "vertexIDs");
    std::string tmp_buf;
    for(size_t i = 0; i < vertexIDs.size(); i++){
      if(vertexIDs[i] == '[') continue;
      else if(vertexIDs[i]==','){
	istringstream tmp_buf_stream(tmp_buf);
	int64_t vertexID;
	tmp_buf_stream >> vertexID;
	listOfVertices.push_back(vertexID);
	tmp_buf = "";
      } else if(vertexIDs[i] == ']'){
	istringstream tmp_buf_stream(tmp_buf);
	int64_t vertexID;
	tmp_buf_stream >> vertexID;
	listOfVertices.push_back(vertexID);
	tmp_buf = "";
	break;
      } else {
	tmp_buf += vertexIDs[i];
      }
    }
  }
   
  size_t last = ( num_vertexIDs < 100) ? num_vertexIDs : 100;
    
  // just to provide feed-back:
  std::ostringstream oss;
  oss << ( decoded ? "parameter base64-decoded: " : "parameter old format: ") 
      << num_vertexIDs << " vertexIDs" << std::endl << "  ids = ";
  for ( size_t idx = 0; idx < last; idx++) {
    if ( idx) oss << ", ";
    oss << listOfVertices[ idx];
  }
  if ( num_vertexIDs > 100) {
    oss << ", ..., " << listOfVertices[ num_vertexIDs - 1];
  }
  LOGGER << oss.str() << " ." << std::endl;

  std::stringstream sessionIDStr;
  sessionIDStr << sessionID;
  
  rvGetResultFromVerticesID _return_;

  DataLayerAccess::Instance()->getResultFromVerticesID(_return_ ,sessionIDStr.str() ,modelID ,analysisID ,timeStep ,resultID ,listOfVertices);
  
  std::vector<int64_t> resultVertexIDs;
  std::vector<double>  resultValues;
		  
  for(size_t i = 0; i < _return_.result_list.size(); i++){
	  resultVertexIDs.push_back(_return_.result_list[i].id);
	  for(size_t j = 0; j < _return_.result_list[i].value.size(); j++)
	    resultValues.push_back(_return_.result_list[i].value[j]);
  }
		  
  // Pack into string
  if(_return_.result_list.size() > 0){
	std::string result;
    std::ostringstream oss;
    size_t nVertices = _return_.result_list.size();
    size_t nElements = _return_.result_list[0].value.size();
    oss.write((char*)&nVertices, sizeof(int64_t));
    oss.write((char*)&nElements, sizeof(int64_t));
    oss.write((char*)(&resultVertexIDs[0]), sizeof(int64_t)*resultVertexIDs.size());
    oss.write((char*)(&resultValues[0]),    sizeof(double)*resultValues.size());
    result = oss.str();
    _return.__set_data(result);  
  } else {
	std::string result;
    std::ostringstream oss;
    int64_t zero = 0;
    oss.write((char*)&zero, sizeof(size_t));
    oss.write((char*)&zero, sizeof(size_t));
    result = oss.str();
    _return.__set_data(result); 
  }
		  
  // Generate example result data
  if (0)
    {
      std::vector<int64_t> resultVertexIDs;
      std::vector<double>  resultValues;
		      
      // vertex #1
      resultVertexIDs.push_back(6);
      resultValues.push_back(-0.105564);
      resultValues.push_back(-0.287896);
      resultValues.push_back(-0.377642);
      // vertex #2
      resultVertexIDs.push_back(7);
      resultValues.push_back( 0.259839);
      resultValues.push_back(-0.366375);
      resultValues.push_back(-0.377652);
		      
      // Pack into string
      std::string result;
      result += "2 3\n";
      result += std::string((char*)(&resultVertexIDs[0]), sizeof(int64_t)*resultVertexIDs.size());
      result += std::string((char*)(&resultValues[0]),    sizeof(double)*resultValues.size());
      _return.__set_data(result); 
    }
		  
  _return.__set_result( (Result::type)VAL_SUCCESS );
		  
  LOGGER                                             << std::endl;
  LOGGER << "Output:"                                << std::endl;
  LOGGER << "  result : "   << _return.result        << std::endl;
  LOGGER << "  data   : \n" << Hexdump(_return.data, 128) << std::endl;
}

static bool isMeshTypeImplemented(MeshInfo& meshInfo) {
	switch(meshInfo.elementType.shape){
	case ElementShapeType::type::SphereElement:
	case ElementShapeType::type::TetrahedraElement:
	case ElementShapeType::type::PointElement:
	case ElementShapeType::type::LineElement:
	case ElementShapeType::type::TriangleElement:
		std::cout << " Mesh Type Supported.\n";
		return true;
	  
	default:
		std::cout << " Mesh Type NOT Supported.\n";
		return false;
	}
}

void QueryManagerServer::ManageGetMeshDrawData( Query_Result& _return, const SessionID sessionID, const std::string& query ) {
  // Parse query JSON
  std::istringstream ss(query);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);

  std::string name        = pt.get<std::string>("name");
  std::string modelID     = pt.get<std::string>("modelID");
  std::string analysisID  = pt.get<std::string>("analysisID");
  double      timeStep    = pt.get<double>("timeStep");
  std::string meshName    = pt.get<std::string>("meshID");
  
  
  std::string dl_sessionID = GetDataLayerSessionID( sessionID);

  /*std::cout << "S " << sessionID  << std::endl;
  std::cout << "M " << modelID    << std::endl;
  std::cout << "A " << analysisID << std::endl;
  std::cout << "T " << timeStep << std::endl;
  std::cout << "M " << meshName << std::endl;*/
  
  const bool test = false;
  if(test){
	  //int nFaces = 12;
	  //int nVertices = 8;

	  float vertices[] = 
	  {

		// # Vertex 0
		-1.0f,  -1.0f,  -1.0f,
		-0.57f, -0.57f, -0.57f,

		// # Vertex 1
		 1.0f,  -1.0f,  -1.0f,
		 0.57f, -0.57f, -0.57f,

		// # Vertex 2
		 1.0f,  -1.0f,   1.0f,
		 0.57f, -0.57f,  0.57f,

		// # Vertex 3
		-1.0f,  -1.0f,   1.0f,
		-0.57f, -0.57f,  0.57f,

		// # Vertex 4
		-1.0f,   1.0f,  -1.0f,
		-0.57f,  0.57f, -0.57f,

		// # Vertex 5
		 1.0f,   1.0f,  -1.0f,
		 0.57f,  0.57f, -0.57f,

		// # Vertex 6
		 1.0f,   1.0f,   1.0f,
		 0.57f,  0.57f,  0.57f,

		// # Vertex 7
		-1.0f,   1.0f,   1.0f,
		-0.57f,  0.57f,  0.57f,

	  };
	  int faces [] = 
	  {
		// # Strip 0
		0, 4, 1, 5, 2, 6, 3, 7, 0, 4, -1,

		// # Strip 1
		0, 1, 3, 2, -1,

		// # Strip 2
		4, 5, 7, 6, -1,
	  };

	  char description[] =
		"# TEST PLY DATA                    \n"
		"VertexDefinition = position, normal\n"
		"vertexDefinitionType = float       \n"

		"OgLPrimitiveRestartIndex = -1      \n";

	  VELaSSCo::RTFormat::File file;

	  file.header.magic[0] = 'V';
	  file.header.magic[1] = 'E';
	  file.header.magic[2] = 'L';
	  file.header.magic[3] = 'a';
	  file.header.magic[4] = 'S';
	  file.header.magic[5] = 'S';
	  file.header.magic[6] = 'C';
	  file.header.magic[7] = 'o';

	  file.header.version = 100;
	  file.header.descriptionBytes = sizeof(description);

	  file.header.metaBytes = 0;

	  file.header.vertexDefinitionsBytes  = sizeof(vertices);
	  file.header.vertexAttributesBytes   = 0;
	  file.header.edgeDefinitionsBytes    = 0;
	  file.header.edgeAttributesBytes     = 0;
	  file.header.faceDefinitionsBytes    = sizeof(faces);
	  file.header.faceAttributesBytes     = 0;
	  file.header.cellDefinitionsBytes    = 0;
	  file.header.cellAttributesBytes     = 0;

	  file.data.description       = (uint8_t*)description;
	  file.data.meta              = 0;

	  file.data.vertexDefinitions = (uint8_t*)vertices;
	  file.data.vertexAttributes  = 0;
	  file.data.edgeDefinitions   = 0;
	  file.data.edgeAttributes    = 0;
	  file.data.faceDefinitions   = (uint8_t*)faces;
	  file.data.faceAttributes    = 0;
	  file.data.cellDefinitions   = 0;
	  file.data.cellAttributes    = 0;

	  // Pack into string
	  std::ostringstream oss;
	  oss << file;  

	  _return.__set_data( oss.str() ); 
	  _return.__set_result( (Result::type)VAL_SUCCESS );
  } else {
	  	  
	  std::string error_str;

	  // from the mesh name, get the Mesh number
	  // eventually the Mesh information could have been cached ...

	  std::cout << "looking for the Mesh " << meshName << " in order to get it's id" << std::endl;
	  rvGetListOfMeshes _return_;
	  DataLayerAccess::Instance()->getListOfMeshes( _return_,
							dl_sessionID, modelID, analysisID, timeStep);
	  MeshInfo meshInfo;
	  meshInfo.meshNumber = -1;      //<<< To mark the meshInfo as non-initalized.
	  std::string elementType = "";
	  if ( _return_.meshInfos.size() == 0) {
		_return.__set_result( (Result::type)VAL_NO_MESH_INFORMATION_FOUND);
		error_str = "There is no mesh metadata.";
	  } else {
		for ( std::vector< MeshInfo>::iterator it = _return_.meshInfos.begin();
			  it != _return_.meshInfos.end(); it++) {
		  if ( AreEqualNoCase( it->name, meshName)) {
			meshInfo = (*it);
			break;
		  }
		}
		if ( meshInfo.meshNumber == -1 ) { // not found
		  error_str = "Mesh name " + meshName + " not in metadata.";
		  std::cout << error_str << std::endl;
		}
	  }
	   
	  VELaSSCo::RTFormat::File binaryMesh;
	  if ( error_str.length() == 0) {
		  
		  std::cout << "====>\n";
		if( isMeshTypeImplemented(meshInfo) ){
		  
			try {
			  rvGetCoordinatesAndElementsFromMesh _return_;
			  DataLayerAccess::Instance()->getCoordinatesAndElementsFromMesh( _return_, dl_sessionID, modelID ,analysisID ,timeStep, meshInfo );
			  
			  std::vector<std::vector<Vertex>> vertex_list;
			  if(meshInfo.name == "p2p contacts"){
				rvGetListOfVerticesFromMesh _return_vertices;
			    DataLayerAccess::Instance()->getListOfVerticesFromMesh( _return_vertices, dl_sessionID, modelID ,analysisID ,timeStep, 1 );
			    vertex_list.push_back( _return_vertices.vertex_list );
			    DataLayerAccess::Instance()->getListOfVerticesFromMesh( _return_vertices, dl_sessionID, modelID ,analysisID ,timeStep, meshInfo.meshNumber );
			    vertex_list.push_back( _return_vertices.vertex_list );
			  } else if(meshInfo.name == "p2w contacts"){
				rvGetListOfVerticesFromMesh _return_vertices;
			    DataLayerAccess::Instance()->getListOfVerticesFromMesh( _return_vertices, dl_sessionID, modelID ,analysisID ,timeStep, 1 );
			    vertex_list.push_back( _return_vertices.vertex_list );
			    DataLayerAccess::Instance()->getListOfVerticesFromMesh( _return_vertices, dl_sessionID, modelID ,analysisID ,timeStep, meshInfo.meshNumber );
			    vertex_list.push_back( _return_vertices.vertex_list );
			  } else {
				rvGetListOfVerticesFromMesh _return_vertices;
				DataLayerAccess::Instance()->getListOfVerticesFromMesh( _return_vertices, dl_sessionID, modelID ,analysisID ,timeStep, meshInfo.meshNumber );
			    vertex_list.push_back( _return_vertices.vertex_list );
			  }
			  
			  GraphicsModule::getInstance()->fromatMeshForDrawing( binaryMesh, meshInfo, vertex_list, _return_.element_list, _return_.element_attrib_list, _return_.element_group_info_list );
			} catch ( TException &e) {
			  std::cout << "CATCH_ERROR 1: " << e.what() << std::endl;
			} catch ( exception &e) {
			  std::cout << "CATCH_ERROR 2: " << e.what() << std::endl;
			}
			std::cout << "==========>\n";
		} else {
			error_str = "The mesh type is not implemented, yet!\n";
		}
	}

	if ( error_str.length() == 0) {
	  // Pack into string
	  std::ostringstream oss;
	  oss << binaryMesh;

	  _return.__set_data( oss.str() ); 
	  _return.__set_result( (Result::type)VAL_SUCCESS );
	} else {
	  _return.__set_result( (Result::type)VAL_UNKNOWN_ERROR);
	  _return.__set_data( error_str );
	}
			  
	LOGGER                                             		<< std::endl;
	LOGGER << "Output:"                                		<< std::endl;
	LOGGER << "  result : "   << _return.result        		<< std::endl;
	LOGGER << "  mesh = ( " << _return.data.length() 		<< " bytes)" << std::endl;
	if ( error_str.length() == 0) {
	  LOGGER << "  data   : \n" << Hexdump( _return.data, 128) << std::endl;
	} else {
	  LOGGER << "  error  : \n" << Hexdump(_return.data, 128) << std::endl;
	}
  }
}

const char *QueryManagerServer::getStrFromElementType( const ElementShapeType::type &elem) {
  const char *str_elem = "Unkown";
  if ( elem == ElementShapeType::type::UnknownElement)	             str_elem = "Unknown";	      	
  else if ( elem == ElementShapeType::type::PointElement)	     str_elem = "Point";	        
  else if ( elem == ElementShapeType::type::LineElement)	     str_elem = "Line";	        
  else if ( elem == ElementShapeType::type::TriangleElement)	     str_elem = "Triangle";	        
  else if ( elem == ElementShapeType::type::QuadrilateralElement)    str_elem = "Quadrilateral";    
  else if ( elem == ElementShapeType::type::TetrahedraElement)       str_elem = "Tetrahedra";       
  else if ( elem == ElementShapeType::type::HexahedraElement)	     str_elem = "Hexahedra";        
  else if ( elem == ElementShapeType::type::PrismElement)	     str_elem = "Prism";	        
  else if ( elem == ElementShapeType::type::PyramidElement)	     str_elem = "Pyramid";	        
  else if ( elem == ElementShapeType::type::SphereElement)	     str_elem = "Sphere";	        
  else if ( elem == ElementShapeType::type::CircleElement)	     str_elem = "Circle";	        
  else if ( elem == ElementShapeType::type::ComplexParticleElement)  str_elem = "ComplexParticle";  
  return str_elem;
}

void QueryManagerServer::ManageGetListOfMeshes( Query_Result &_return, const SessionID sessionID, const std::string& query) {
  // Parse query JSON
  std::istringstream ss(query);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  
  std::string name       = pt.get<std::string>("name");
  std::string modelID    = pt.get<std::string>( "modelID");
  std::string analysisID = pt.get<std::string>( "analysisID");
  double stepValue       = pt.get< double>( "stepValue");
  
  std::string dl_sessionID = GetDataLayerSessionID( sessionID);

  std::cout << "S   " << sessionID        << std::endl;
  std::cout << "dlS " << dl_sessionID     << std::endl;
  std::cout << "M  -" << modelID          << "-" << std::endl;
  std::cout << "An -" << analysisID       << "-" << std::endl;
  std::cout << "Sv -" << stepValue       << "-" << std::endl;

  rvGetListOfMeshes _return_;
  DataLayerAccess::Instance()->getListOfMeshes( _return_,
						dl_sessionID, modelID, analysisID, stepValue);
  
  std::cout << _return_ << std::endl;

  if ( _return_.status == "Error") {
    const std::string not_found( "Not found");
    if ( AreEqualNoCaseSubstr( _return_.report, not_found, not_found.size())) {
      _return.__set_result( (Result::type)VAL_NO_MODEL_MATCHES_PATTERN);
    } else {
      _return.__set_result( (Result::type)VAL_UNKNOWN_ERROR);
    }
    _return.__set_data( _return_.report);
  } else { // status == "Ok"
    if ( _return_.meshInfos.size() == 0) {
      _return.__set_result( (Result::type)VAL_NO_MESH_INFORMATION_FOUND);
    } else {
      _return.__set_result( (Result::type)VAL_SUCCESS );
      // process data:
      // foreach model in _return_.meshInfos
      //   return { model.name, model.fullpath}
      // _return.__set_data( _return_.meshInfos.srt());
      std::ostringstream oss;
      oss << "NumberOfMeshes: " << _return_.meshInfos.size() << std::endl;
      // C++11 : for ( auto &it : _return_.meshInfos)
      // char hex_string[ 1024];
      for ( std::vector< MeshInfo>::iterator it = _return_.meshInfos.begin();
          it != _return_.meshInfos.end(); it++) {
	oss << "Name: " << it->name << std::endl;
	oss << "ElementType: " << getStrFromElementType( it->elementType.shape) << std::endl;
	oss << "NumberOfVerticesPerElement: " <<  it->elementType.num_nodes << std::endl;
	oss << "NumberOfVertices: " << it->nVertices << std::endl;
	oss << "NumberOfElements: " << it->nElements << std::endl;
	oss << "Units: " << it->meshUnits << std::endl;
	oss << "Color: " << it->meshColor << std::endl;
	oss << "MeshNumber: " << it->meshNumber << std::endl;
	oss << "CoordsName: " << it->coordsName << std::endl;

      }
      _return.__set_data( oss.str());
    }
  }
		  
  LOGGER                                             << std::endl;
  LOGGER << "Output:"                                << std::endl;
  LOGGER << "  result : "   << _return.result        << std::endl;
  // LOGGER << "  data   : \n" << Hexdump(_return.data) << std::endl;
  LOGGER << "  data   : \n" << _return.data << std::endl;
} // ManageGetListOfMeshes

void QueryManagerServer::ManageGetListOfAnalyses( Query_Result &_return, const SessionID sessionID, const std::string& query) {
  // Parse query JSON
  std::istringstream ss(query);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  
  std::string name       = pt.get<std::string>("name");
  std::string modelID    = pt.get<std::string>( "modelID");
  
  std::string dl_sessionID = GetDataLayerSessionID( sessionID);

  std::cout << "S   " << sessionID        << std::endl;
  std::cout << "dlS " << dl_sessionID     << std::endl;
  std::cout << "M  -" << modelID          << "-" << std::endl;

  rvGetListOfAnalyses _return_;
  DataLayerAccess::Instance()->getListOfAnalyses( _return_,
						  dl_sessionID, modelID);
  
  std::cout << _return_ << std::endl;
  
  if ( _return_.status == "Error") {
    const std::string not_found( "Not found");
    if ( AreEqualNoCaseSubstr( _return_.report, not_found, not_found.size())) {
      _return.__set_result( (Result::type)VAL_NO_ANALYSIS_INFORMATION_FOUND);
    } else {
      _return.__set_result( (Result::type)VAL_UNKNOWN_ERROR);
    }
    _return.__set_data( _return_.report);
  } else { // status == "Ok"
    if ( _return_.analyses.size() == 0) {
      _return.__set_result( (Result::type)VAL_NO_MODEL_MATCHES_PATTERN);
    } else {
      _return.__set_result( (Result::type)VAL_SUCCESS );
      // process data:
      std::ostringstream oss;
      for ( std::vector< std::string>::iterator it = _return_.analyses.begin();
          it != _return_.analyses.end(); it++) {
	oss << *it << std::endl;
      }
      _return.__set_data( oss.str());
    }
  }
		  
  LOGGER                                             << std::endl;
  LOGGER << "Output:"                                << std::endl;
  LOGGER << "  result : "   << _return.result        << std::endl;
  // LOGGER << "  data   : \n" << Hexdump(_return.data) << std::endl;
  LOGGER << "  data   : \n" << _return.data << std::endl;

} // ManageGetListOfAnalyses

void QueryManagerServer::ManageGetListOfTimeSteps( Query_Result &_return, const SessionID sessionID, const std::string& query) {
  // Parse query JSON
  std::istringstream ss(query);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  
  std::string name       = pt.get<std::string>( "name");
  std::string modelID    = pt.get<std::string>( "modelID"); 
  std::string analysisID = pt.get<std::string>( "analysisID");
 
  std::string dl_sessionID = GetDataLayerSessionID( sessionID);

  std::cout << "S   " << sessionID        << std::endl;
  std::cout << "dlS " << dl_sessionID     << std::endl;
  std::cout << "M  -" << modelID          << "-" << std::endl;
  std::cout << "An -" << analysisID       << "-" << std::endl;

  rvGetListOfTimeSteps _return_;
  DataLayerAccess::Instance()->getListOfTimeSteps( _return_,
						   dl_sessionID, modelID, analysisID,
						   "ALL", 0, NULL);
  
  // std::cout << _return_ << std::endl;
  
  if ( _return_.status == "Error") {
    const std::string not_found( "Not found");
    if ( AreEqualNoCaseSubstr( _return_.report, not_found, not_found.size())) {
      _return.__set_result( (Result::type)VAL_NO_STEP_INFORMATION_FOUND);
    } else {
      _return.__set_result( (Result::type)VAL_UNKNOWN_ERROR);
    }
    _return.__set_data( _return_.report);
  } else { // status == "Ok"
    if ( _return_.time_steps.size() == 0) {
      _return.__set_result( (Result::type)VAL_NO_MODEL_MATCHES_PATTERN);
    } else {
      _return.__set_result( (Result::type)VAL_SUCCESS );
      // process data:
      // std::ostringstream oss;
      // // has to be passed as binary !!!!
      // for ( std::vector< double>::iterator it = _return_.time_steps.begin();
      //     it != _return_.time_steps.end(); it++) {
      // 	oss << *it << std::endl;
      // }
      // _return.__set_data( oss.str());
      _return.__set_data( std::string( ( char *)_return_.time_steps.data(), _return_.time_steps.size() * sizeof( double)));
    }
  }
		  
  LOGGER                                             << std::endl;
  LOGGER << "Output:"                                << std::endl;
  LOGGER << "  result : "   << _return.result        << std::endl;
  // LOGGER << "  data   : \n" << Hexdump(_return.data) << std::endl;
  LOGGER << "  data   : \n" << Hexdump( _return.data) << std::endl;
  LOGGER << "  # steps: "   << _return_.time_steps.size() << std::endl;
  if ( _return_.time_steps.size() > 0)
    LOGGER << "  step 0 : " << _return_.time_steps[ 0] << std::endl;
  if ( _return_.time_steps.size() > 1)
    LOGGER << "  step 1 : " << _return_.time_steps[ 1] << std::endl;
  if ( _return_.time_steps.size() > 2)
    LOGGER << "  step 2 : " << _return_.time_steps[ 2] << std::endl;

} // ManageGetListOfTimeSteps

void QueryManagerServer::ManageGetListOfResults( Query_Result &_return, const SessionID sessionID, const std::string& query) {
  // Parse query JSON
  std::istringstream ss(query);
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);
  
  std::string name       = pt.get<std::string>("name");
  std::string modelID    = pt.get<std::string>( "modelID");
  std::string analysisID = pt.get<std::string>( "analysisID");
  double stepValue       = pt.get< double>( "stepValue");
  
  std::string dl_sessionID = GetDataLayerSessionID( sessionID);

  std::cout << "S   " << sessionID        << std::endl;
  std::cout << "dlS " << dl_sessionID     << std::endl;
  std::cout << "M  -" << modelID          << "-" << std::endl;
  std::cout << "An -" << analysisID       << "-" << std::endl;
  std::cout << "Sv -" << stepValue       << "-" << std::endl;

  rvGetListOfResults _return_;
  DataLayerAccess::Instance()->getListOfResultsFromTimeStepAndAnalysis( _return_,
									dl_sessionID, 
									modelID, analysisID, stepValue);
  
  std::cout << _return_ << std::endl;

  if ( _return_.status == "Error") {
    const std::string not_found( "Not found");
    if ( AreEqualNoCaseSubstr( _return_.report, not_found, not_found.size())) {
      _return.__set_result( (Result::type)VAL_NO_RESULT_INFORMATION_FOUND);
    } else {
      _return.__set_result( (Result::type)VAL_UNKNOWN_ERROR);
    }
    _return.__set_data( _return_.report);
  } else { // status == "Ok"
    if ( _return_.result_list.size() == 0) {
      _return.__set_result( (Result::type)VAL_NO_MODEL_MATCHES_PATTERN);
    } else {
      _return.__set_result( (Result::type)VAL_SUCCESS );
      // process data:
      // foreach model in _return_.result_list
      //   return { model.name, model.fullpath}
      // _return.__set_data( _return_.result_list.srt());
      std::ostringstream oss;
      oss << "NumberOfResults: " << _return_.result_list.size() << std::endl;
      // C++11 : for ( auto &it : _return_.result_list)
      
      /* the information returned is ResultType, NumberOfComponents, ComponentNames, Location, GaussPointName, CoordinatesName, Units, ... */
      // char hex_string[ 1024];
      for ( std::vector< ResultInfo>::iterator it = _return_.result_list.begin();
          it != _return_.result_list.end(); it++) {
	oss << "Name: " << it->name << std::endl;
	oss << "ResultType: " << it->type << std::endl;
	oss << "NumberOfComponents: " <<  it->numberOfComponents << std::endl;
	oss << "ComponentNames: ";
	for ( size_t i = 0; i < it->componentNames.size(); i++) {
	  if ( i) oss << " ";
	  oss << "{" << it->componentNames[ i] << "}";
	}
	oss << std::endl;
	oss << "Location: " << it->location << std::endl;
	oss << "GaussPointName: " << it->gaussPointName << std::endl;
	oss << "CoordinatesName: " << it->coordinatesName << std::endl;
	oss << "Units: " << it->units << std::endl;
	// not yet ...
	oss << "ResultNumber: " << it->resultNumber << std::endl;
      }
      _return.__set_data( oss.str());
    }
  }
		  
  LOGGER                                             << std::endl;
  LOGGER << "Output:"                                << std::endl;
  LOGGER << "  result : "   << _return.result        << std::endl;
  // LOGGER << "  data   : \n" << Hexdump(_return.data) << std::endl;
  LOGGER << "  data   : \n" << _return.data << std::endl;
} // ManageGetListOfMeshes
