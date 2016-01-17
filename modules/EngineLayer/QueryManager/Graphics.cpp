#include <sstream>
#include <stddef.h>  // defines NULL
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Helpers.h"
#include "Graphics.h"

// Global static pointer used to ensure a single instance of the class.
GraphicsModule *GraphicsModule::m_pInstance = NULL;

GraphicsModule *GraphicsModule::getInstance() {
  // Only allow one instance of class to be generated.
  std::cout << "in GraphicsModule::getInstance" << std::endl;
  if ( !m_pInstance) {
    m_pInstance = new GraphicsModule;
  }
  return m_pInstance;
}

void GraphicsModule::fromatMeshForDrawing(VELaSSCo::RTFormat::File& _return_, const MeshInfo& meshInfo, 
  const std::vector<Vertex>& vertices, const std::vector<Element>& elements, 
  const std::vector<ElementAttrib>& elementAttribs, const std::vector<ElementGroup>& elementGroupInfos)
{	
	const size_t num_vertices    = vertices.size();
	const size_t num_elements    = elements.size();
	const size_t num_attribs     = elementAttribs.size();
	//size_t num_groupInfos  = elementInfos.size();
	
// due to wrong data in the table.
#define _TODO_SHOULD_BE_REMOVED
#ifdef  _TODO_SHOULD_BE_REMOVED
// TODO Use ther meshInfo
    int64_t max_vertex_id = -1;
    for(size_t i = 0; i < elements.size(); i++)
      if(elements[i].id > max_vertex_id)
        max_vertex_id = elements[i].id;
    for(size_t i = 0; i < elementAttribs.size(); i++)
      if(elementAttribs[i].id > max_vertex_id){
		  max_vertex_id = elementAttribs[i].id;
	  }
	std::vector<float> ret_vertices( 4 * (max_vertex_id + 1) );
#else
    //std::vector<float> ret_vertices( /*max_vertex_id*/ );
#endif
	

	for(size_t i = 0; i < elements.size(); i++){
		int64_t vertexID = elements[i].nodes_ids.size() > 0 ? static_cast<int64_t>( elements[i].nodes_ids[0] ) : -1;
		if(vertexID > 0){
		  ret_vertices[4 * elements[i].id + 0] = vertices[vertexID].x;
		  ret_vertices[4 * elements[i].id + 1] = vertices[vertexID].y;
		  ret_vertices[4 * elements[i].id + 2] = vertices[vertexID].z;
		  ret_vertices[4 * elements[i].id + 3] = 1.0f;
	    }
	}
	
	for(size_t i = 0; i < elementAttribs.size(); i++){
		ret_vertices[4 * elementAttribs[i].id + 3] = elementAttribs[i].value.size() > 0 ? elementAttribs[i].value[0] : 1.0f;
	}
	
	char description[] =
		"# TEST PLY DATA                             \n"
		"VertexDefinition         = position, radius \n"
		"vertexDefinitionsType    = float3, float    \n"	
		"OgLPrimitiveRestartIndex = -1               \n";
	  
	  _return_.header.magic[0] = 'V';
	  _return_.header.magic[1] = 'E';
	  _return_.header.magic[2] = 'L';
	  _return_.header.magic[3] = 'a';
	  _return_.header.magic[4] = 'S';
	  _return_.header.magic[5] = 'S';
	  _return_.header.magic[6] = 'C';
	  _return_.header.magic[7] = 'o';

	  _return_.header.version = 100;
	  _return_.header.descriptionBytes = sizeof(description);

	  _return_.header.metaBytes = 0;

	  _return_.header.vertexDefinitionsBytes  = static_cast<uint64_t>(ret_vertices.size() * sizeof(float));
	  _return_.header.vertexAttributesBytes   = 0;
	  _return_.header.edgeDefinitionsBytes    = 0;
	  _return_.header.edgeAttributesBytes     = 0;
	  _return_.header.faceDefinitionsBytes    = 0;
	  _return_.header.faceAttributesBytes     = 0;
	  _return_.header.cellDefinitionsBytes    = 0;
	  _return_.header.cellAttributesBytes     = 0;

	  _return_.data.description.resize( _return_.header.descriptionBytes );
	  _return_.data.meta.resize(0);
      _return_.data.vertexDefinitions.resize( _return_.header.vertexDefinitionsBytes );
	  _return_.data.vertexAttributes.resize(0);
	  _return_.data.edgeDefinitions.resize(0);
	  _return_.data.edgeAttributes.resize(0);
	  _return_.data.faceDefinitions.resize(0);
	  _return_.data.faceAttributes.resize(0);
	  _return_.data.cellDefinitions.resize(0);
	  _return_.data.cellAttributes.resize(0);
	  
	  memcpy(_return_.data.description.data(), description, _return_.header.descriptionBytes);
	  memcpy((char*)_return_.data.vertexDefinitions.data(), (const char*)ret_vertices.data(), _return_.header.vertexDefinitionsBytes);
}
