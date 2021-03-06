// EDM_plug_in_tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "random_points.h"
#include "elementID_of_random_points.h"
#include "O:/projects/VELaSSCo/SVN_src/modules/DataLayer/Storage/EDM_server_dll/EDM_server_dll.h"
//#include "..\..\..\EngineLayer\QueryManager\analytics.h"
//#include "EDManalytics.h"


VELaSSCoSMClient *clp = NULL;

std::string getResultOnVertices(std::string sessionID,
   std::string modelID,
   std::string analysisID,
   double      timeStep,
   std::string resultID,
   std::string listOfVertices)
{

   //rvOpenModel rvOM;
   //string modelID_of_VELaSSCo_HbaseBasicTest_part_1;
   //VELaSSCoSM::rvGetResultFromVerticesID_B results;
   //vector<int64_t> vertexIDs;

   //clp->FindModel(rvOM, sessionID, "VELaSSCo_HbaseBasicTest_part_1", "read");
   //modelID_of_VELaSSCo_HbaseBasicTest_part_1 = rvOM.modelID;
   //clp->GetResultFromVerticesID(results, sessionID, modelID_of_VELaSSCo_HbaseBasicTest_part_1, "", vertexIDs, "Height", 1.0, "geometry");

   string resultString;
   //for (vector<VertexResult>::iterator resIter = results.vertexResults.begin(); resIter != results.vertexResults.end(); resIter++) {
   //   char cb[2048], *np;
   //   int length = sprintf(cb, "%d ", resIter->vertexID); np = cb + length;
   //   for (vector<double>::iterator value = resIter->resuls.begin(); value != resIter->resuls.end(); value++) {
   //      length = sprintf(np, " %f", *value); np = np + length;
   //   }
   //   sprintf(np, "\n");
   //   resultString += cb;
   //}
   return resultString;
}


void GetCoordinatesAndElementsFromMesh(VELaSSCoSMClient &client, string sessionID, char *modelName, string modelID)
{
   int endTime, startTime;
   rvGetCoordinatesAndElementsFromMesh meshInfoRV;

   printf("\n--->GetCoordinatesAndElementsFromMesh - \"%s\"\n", modelName);
   VELaSSCoSM::MeshInfo meshInfo;
   startTime = GetTickCount();
   client.GetCoordinatesAndElementsFromMesh(meshInfoRV, sessionID, modelID, "Kratos", 21, meshInfo);
   endTime = GetTickCount();
   printf("Elapsed time for GetCoordinatesAndElementsFromMesh is %d milliseconds\n", endTime - startTime);
   if (strncmp(meshInfoRV.status.data(), "Error", 5) == 0) {
      printf("Error message: \"%s\"\n", meshInfoRV.report.data());
   } else {
      printf("Query report:\n%s\n\n", meshInfoRV.report.data());
      //OLI
      printf("Number of vertices transferred: %12llu\n", meshInfoRV.vertex_list.size());
      for (int i = 0; i < 20 && i < meshInfoRV.vertex_list.size(); i++) {
         VELaSSCoSM::Vertex v = meshInfoRV.vertex_list[i];
         printf("%12llu   %Lf  %Lf  %Lf\n", v.id, v.x, v.y, v.z);
      }
      printf("Number of elements transferred: %12llu\n", meshInfoRV.element_list.size());
      for (int i = 0; i < 20 && i < meshInfoRV.element_list.size(); i++) {
         VELaSSCoSM::Element e = meshInfoRV.element_list[i];
         printf("%12llu   ", e.id);
         for (int j = 0; j < meshInfoRV.element_list[i].nodes_ids.size(); j++) printf("  %10llu", meshInfoRV.element_list[i].nodes_ids[j]);
         printf("\n");
      }
   }
}

void GetResultFromVerticesID(VELaSSCoSMClient &client, string sessionID, char *modelName, string modelID)
{
   int endTime, startTime;
   rvGetResultFromVerticesID verticesResultRV;
   vector<int64_t> listOfVertices;

   listOfVertices.push_back(63327);
   listOfVertices.push_back(63699);
   listOfVertices.push_back(63707);
   listOfVertices.push_back(64285);
   listOfVertices.push_back(123400);

   printf("\n--->GetResultFromVerticesID - \"%s\"\n", modelName);
   startTime = GetTickCount();
   client.GetResultFromVerticesID(verticesResultRV, sessionID, modelID, "Kratos", 21, "PRESSURE", listOfVertices);
   endTime = GetTickCount();
   printf("Elapsed time for GetResultFromVerticesID is %d milliseconds\n", endTime - startTime);
   printf("Return status: %s\n", verticesResultRV.status.data());
   if (strncmp(verticesResultRV.status.data(), "Error", 5) == 0) {
      printf("Error message: \"%s\"\n", verticesResultRV.report.data());
   } else {
      int nRes = 0;
//#define BLOB 1
#ifdef BLOB
      printf("%s has %d results.\n", modelName, verticesResultRV.n_result_records);
      char *cp = (char*)verticesResultRV.result_array.data();
      for (int i = 0; i < 16 && i < verticesResultRV.n_result_records; i++) {
         EDMVD::ResultOnVertex *vertexResult = (EDMVD::ResultOnVertex*)cp;
         printf("%10llu %Lf\n", vertexResult->id, vertexResult->value[0]);
         cp += verticesResultRV.result_record_size;
      }
#else
      for (vector<VELaSSCoSM::ResultOnVertex>::iterator resIter = verticesResultRV.result_list.begin(); resIter != verticesResultRV.result_list.end(); resIter++) {
         vector<double>::iterator valuesIter = resIter->value.begin();
         if (nRes++ > 16) break;
         printf("%10llu %Lf\n", resIter->id, *valuesIter);
      }
#endif
      printf(verticesResultRV.report.data());
   }
}



void GetBoundaryOfLocalMesh(VELaSSCoSMClient &client, string sessionID, char *modelName, string modelID)
{
   int endTime, startTime;

   rvGetBoundaryOfLocalMesh boundaryRV;
   printf("\n--->GetBoundaryOfLocalMesh - \"%s\"\n", modelName);
   startTime = GetTickCount();
   client.GetBoundaryOfLocalMesh(boundaryRV, sessionID, modelID, "meshID", "Kratos", 21);
   endTime = GetTickCount();
   printf("Elapsed time for GetBoundaryOfLocalMesh is %d milliseconds\n", endTime - startTime);
   if (strncmp(boundaryRV.status.data(), "Error", 5) == 0) {
      printf("Error message: \"%s\"\n", boundaryRV.report.data());
   } else {
      printf("Query report:\n%s\n\n\n", boundaryRV.report.data());
      printf("Mesh boundary of %s has %d triangles.\n", modelName, boundaryRV.elements.size());
      int nn = 0;
      for (vector<VELaSSCoSM::Triangle>::iterator bufferIter = boundaryRV.elements.begin(); bufferIter != boundaryRV.elements.end() && nn++ < 20; bufferIter++) {
         for (vector<VELaSSCoSM::NodeID>::iterator nodeIter = bufferIter->nodes.begin(); nodeIter != bufferIter->nodes.end(); nodeIter++) {
            printf("%10llu", *nodeIter);
         }
         printf("\n");
      }
   }
}


void GetListOfMeshes(VELaSSCoSMClient &client, string sessionID, char *modelName, string modelID)
{
   int endTime, startTime;
   rvGetListOfMeshes rvMeshes;

   printf("\n--->GetListOfMeshes - \"%s\"\n", modelName);
   startTime = GetTickCount();
   client.GetListOfMeshes(rvMeshes, sessionID, modelID, "Kratos", 21.0);
   endTime = GetTickCount();
   printf("Elapsed time for GetListOfMeshes is %d milliseconds\n", endTime - startTime);
   printf("Return status: %s\n", rvMeshes.status.data());
   printf("%s has %d meshes.\n", modelName, rvMeshes.meshInfos.size());
   for (vector<VELaSSCoSM::MeshInfo>::iterator meshIter = rvMeshes.meshInfos.begin(); meshIter != rvMeshes.meshInfos.end(); meshIter++) {
      printf("   %20s  %10llu  %10llu\n", meshIter->name.data(), meshIter->nElements, meshIter->nVertices);
   }
}
void GetListOfResultsFromTimeStepAndAnalysis(VELaSSCoSMClient &client, string sessionID, char *modelName, string modelID, string analysisID, double timeStep)
{
   int endTime, startTime;
   rvGetListOfResults resultRV;
   printf("\n--->GetListOfResultsFromTimeStepAndAnalysis - \"%s\"\n", modelName);
   startTime = GetTickCount();
   client.GetListOfResultsFromTimeStepAndAnalysis(resultRV, sessionID, modelID, analysisID, timeStep);
   endTime = GetTickCount();
   printf("Elapsed time for GetListOfResultsFromTimeStepAndAnalysis is %d milliseconds\n", endTime - startTime);
   printf("Return status: %s\n", resultRV.status.data());
   printf("%s has %d result headers.\n", modelName, resultRV.result_list.size());
   int nRes = 0;
   if (strncmp(resultRV.status.data(), "Error", 5) == 0) {
      printf("Error message: \"%s\"\n", resultRV.report.data());
   } else {
      for (vector<VELaSSCoSM::ResultInfo>::iterator resIter = resultRV.result_list.begin(); resIter != resultRV.result_list.end(); resIter++) {
         if (nRes++ > 16) break;
         printf("%20s %15s %15s %10d\n", (char*)resIter->name.data(), (char*)resIter->type.data(), (char*)resIter->location.data(), resIter->numberOfComponents);
         for (vector<string>::iterator compName = resIter->componentNames.begin(); compName != resIter->componentNames.end(); compName++) {
            printf("                                     %s\n", compName->data());
         }
      }
   }
}

void GetListOfAnalyses(VELaSSCoSMClient &client, string sessionID, char *modelName, string modelID)
{
   int endTime, startTime;
   rvGetListOfAnalyses rvAnalysisList;
   rvGetListOfTimeSteps rvTimesteps;

   printf("\n--->GetListOfAnalyses - \"%s\"\n", modelName);
   startTime = GetTickCount();
   client.GetListOfAnalyses(rvAnalysisList, sessionID, modelID);
   endTime = GetTickCount();
   printf("Elapsed time for GetListOfAnalyses is %d milliseconds\n", endTime - startTime);
   printf("Return status: %s\n", rvAnalysisList.status.data());
   printf("%s has %d analyses.\n", modelName, rvAnalysisList.analyses.size());
   if (strncmp(rvAnalysisList.status.data(), "Error", 5) == 0) {
      printf("Error message: \"%s\"\n", rvAnalysisList.report.data());
   } else {
      printf("Execution report:\n%s\n\n\n", rvAnalysisList.report.data());
      for (vector<string>::iterator nameIter = rvAnalysisList.analyses.begin(); nameIter != rvAnalysisList.analyses.end(); nameIter++) {
         printf("Analysis name : %s\n", (char*)nameIter->data());
         client.GetListOfTimeSteps(rvTimesteps, sessionID, modelID, *nameIter);
         printf("   %s has %d time steps:\n", (char*)nameIter->data(), rvTimesteps.time_steps.size());
         printf("   %s has the following time steps:\n", (char*)nameIter->data());
         int i = 0;
         for (vector<double>::iterator tsIter = rvTimesteps.time_steps.begin(); tsIter != rvTimesteps.time_steps.end(); tsIter++) {
            printf("%Lf\n", *tsIter);
         }
      }
   }
}


void testListAnalyses(VELaSSCoSMClient &client, string sessionID, char *modelName)
{
   int endTime, startTime;

   rvOpenModel rvOM;
   string modelID;

   client.FindModel(rvOM, sessionID, modelName, "read");
   modelID = rvOM.modelID;

#ifdef ALLEHER
   for (int nPass = 0; nPass < 1; nPass++) {  //100000000

      GetListOfAnalyses(client, sessionID, modelName, modelID);

      rvGetListOfVerticesFromMesh meshRV;
      printf("\n--->GetListOfVerticesFromMesh - \"%s\"\n", modelName);
      startTime = GetTickCount();
      client.GetListOfVerticesFromMesh(meshRV, sessionID, modelID, "", 0.0, 0);
      endTime = GetTickCount();
      printf("Elapsed time for GetListOfVerticesFromMesh is %d milliseconds\n", endTime - startTime);
      printf("Return status: %s\n", meshRV.status.data());
      printf("%s has %d vertices.\n", modelName, meshRV.vertex_list.size());

      //rvGetListOfResults resultRV;
      //printf("\n--->GetListOfResultsFromTimeStepAndAnalysis - \"%s\"\n", modelName);
      //startTime = GetTickCount();
      //client.GetListOfResultsFromTimeStepAndAnalysis(resultRV, sessionID, modelID, "Kratos", 21);
      //endTime = GetTickCount();
      //printf("Elapsed time for GetListOfResultsFromTimeStepAndAnalysis is %d milliseconds\n", endTime - startTime);
      //printf("Return status: %s\n", resultRV.status.data());
      //printf("%s has %d result headers.\n", modelName, resultRV.result_list.size());
      //int nRes = 0;
      //if (strncmp(resultRV.status.data(), "Error", 5) == 0) {
      //   printf("Error message: \"%s\"\n", resultRV.report.data());
      //} else {
      //   for (vector<VELaSSCoSM::ResultInfo>::iterator resIter = resultRV.result_list.begin(); resIter != resultRV.result_list.end(); resIter++) {
      //      if (nRes++ > 16) break;
      //      printf("%20s %10s\n", (char*)resIter->name.data(), (char*)resIter->type.data());
      //   }
      //}

      GetResultFromVerticesID(client, sessionID, modelName, modelID);
      GetCoordinatesAndElementsFromMesh(client, sessionID, modelName, modelID);
      GetResultFromVerticesID(client, sessionID, modelName, modelID);
      GetListOfAnalyses(client, sessionID, modelName, modelID);
      GetListOfMeshes(client, sessionID, modelName, modelID);
      GetBoundaryOfLocalMesh(client, sessionID, modelName, modelID);
      GetListOfResultsFromTimeStepAndAnalysis(client, sessionID, modelName, modelID, "Kratos", 21);

   }

#else /* ALLEHER */


   //GetResultFromVerticesID(client, sessionID, modelName, modelID);
   //GetCoordinatesAndElementsFromMesh(client, sessionID, modelName, modelID);
   //GetResultFromVerticesID(client, sessionID, modelName, modelID);
   //GetListOfAnalyses(client, sessionID, modelName, modelID);
   //GetListOfMeshes(client, sessionID, modelName, modelID);
   GetBoundaryOfLocalMesh(client, sessionID, modelName, modelID);
   //GetListOfResultsFromTimeStepAndAnalysis(client, sessionID, modelName, modelID, "Kratos", 21);

#endif /* ALLEHER */
}


/*===============================================================================================*/
int main(int argc, char* argv[])
/*
Program to test VELaSSCo Data Layer thrift based server. The program has 3 command line parameters:
1. server address
2. server port number
3. Command

Command can eiter be "all" or any of the querynames.
=================================================================================================*/
{
   char *command = "all";
   char *server = "localhost";
   int port = 9090;

   if (argc > 1) server = argv[1];
   if (argc > 2) port = atol(argv[2]);
   if (argc > 3) command = argv[3];

   boost::shared_ptr<TTransport> socket(new TSocket(server, port));
   boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   VELaSSCoSMClient client(protocol);
   clp = &client;

#ifdef WIN32
   WSADATA wsaData = {};
   WORD wVersionRequested = MAKEWORD(2, 2);
   int err = WSAStartup(wVersionRequested, &wsaData);
#endif

   //AnalyticsModule *theAnalyst = new EDManalytics(&client);
   try {
      transport->open();

      rvGetElementOfPointsInSpace rv;
      string sessionID;
      string modelID_of_VELaSSCo_HbaseBasicTest_part_1;
      vector<Point> points;
      Point p;
      rvOpenModel rvOM;

      int nPoints = sizeof(random_points) / sizeof(random_points[0]);
      for (int i = 0; i < 1; i++) {
         printf("\n--->UserLogin\n");
         client.UserLogin(sessionID, "olav", "myRole", "myPassword");
         printf("User logged on with session ID: %s\n", sessionID.data());

         printf("\n--->OpenModel - \"telescope\"\n");
         client.FindModel(rvOM, sessionID, "telescope", "read");
         printf("Returned modelID: %s\n", rvOM.modelID.data());
         printf("Comments: %s\n", rvOM.report.data());
         modelID_of_VELaSSCo_HbaseBasicTest_part_1 = rvOM.modelID;
      }
      //if (strEQL(command, "all") || strEQL(command, "CreateClusterModel")) {
      //   rvCreateClusterModel rvccm;
      //   vector<string> physicalModelNames;

      //   physicalModelNames.push_back("myModel_0");
      //   physicalModelNames.push_back("myModel_1");
      //   printf("\n--->CreateClusterModel:\n");
      //   client.CreateClusterModel(rvccm, sessionID, "FEM_models", "myModel", "fem_schema_velassco", physicalModelNames);
      //   printf("Return status: %s\n", rvccm.status.data());
      //   printf("Comments: %s\n", rvccm.report.data());
      //}
      //double theBoundingBox[6];
      //string returnMsg;
      //theAnalyst->calculateBoundingBox(sessionID, modelID_of_VELaSSCo_HbaseBasicTest_part_1, "", "", 0, NULL, 0, NULL, theBoundingBox, &returnMsg);

      //if (strEQL(command, "all") || strEQL(command, "string_getResultOnVertices")) {
      //   string vertices = getResultOnVertices(sessionID, "0", "analysis", 00.1, "resultID", "LOV");
      //   //printf("getResultOnVertices returns the following string\n");
      //   //printf(vertices.data());
      //}

      if (strEQL(command, "all") || strEQL(command, "GetElementOfPointsInSpace")) {

         for (int i = 0; i < nPoints; i++) {
            p.__set_x(random_points[i][0]); p.__set_y(random_points[i][1]); p.__set_z(random_points[i][2]);
            points.push_back(p);
         }
         printf("\n--->GetElementOfPointsInSpace  - 1000 random points:\n");
         client.GetElementOfPointsInSpace(rv, sessionID, rvOM.modelID, points);

         printf("Return status: %s\n", rv.status.data());
         printf("Comments: %s\n", rv.report.data());
         if (strEQL(rv.status.data(), "OK")) {
            int ix = 0, nCorrect = 0;
            int nErrorFound = false;
            for (std::vector<VELaSSCoSM::Element>::iterator elemId = rv.elements.begin(); elemId != rv.elements.end(); elemId++) {
               if (elemId->id != -1) {
                  if (elemId->id != elementID_of_random_points[ix]) {
                     printf("illegal elementid for point nr %d", ix); nErrorFound++;
                  } else {
                     nCorrect++;
                  }
               }
               ix++;
            }
            printf(nErrorFound ? "GetElementOfPointsInSpace executed with %d errors\n" : "No errors in returned data detected.\n", nErrorFound);
         }
      }

      rvGetBoundaryOfLocalMesh rvm;
      if (strEQL(command, "all") || strEQL(command, "GetBoundaryOfLocalMesh_WithError")) {
         printf("\n--->GetBoundaryOfLocalMesh with wrong model id:\n");
         client.GetBoundaryOfLocalMesh(rvm, sessionID, "", "meshID", "analysisID", 1.0);
         printf("Return status: %s\n", rvm.status.data());
         printf("Comments: %s\n", rvm.report.data());
         if (strcmp(rvm.status.data(), "Error") || strcmp(rvm.report.data(), "Model does not exist.")) {
            printf("Unexpected result from GetBoundaryOfLocalMesh on non existing model\n");
         } else {
            printf("Expected error message returned\n");
         }
      }

      if (strEQL(command, "all") || strEQL(command, "GetBoundaryOfLocalMesh")) {
         printf("\n--->GetBoundaryOfLocalMesh:\n");
         client.GetBoundaryOfLocalMesh(rvm, sessionID, modelID_of_VELaSSCo_HbaseBasicTest_part_1, "meshID", "analysisID", 1.0);
         printf("Return status: %s\n", rvm.status.data());
         printf("Comments: %s\n", rvm.report.data());
      }

      if (strEQL(command, "all") || strEQL(command, "GetResultFromVerticesID_FEM")) {
         //rvGetResultFromVerticesID_B rvB;
         //vector<int64_t> vertexIDs;

         //vertexIDs.push_back(37);
         //printf("\n--->GetResultFromVerticesID - FEM model VELaSSCo_HbaseBasicTest_part_1:\n");
         //client.GetResultFromVerticesID(rvB, sessionID, modelID_of_VELaSSCo_HbaseBasicTest_part_1, "", vertexIDs, "Height", 1.0, "geometry");
         //printf("Return status: %s\n", rvB.status.data());
         //printf("Comments: %s\n", rvB.report.data());
      }

      if (strEQL(command, "multi") || strEQL(command, "GetListOfModels")) {
         rvGetListOfModels modelsInfo;
         rvOpenModel rvOM;

         for (int i = 0; i < 1; i++) {
            printf("\n--->GetListOfModels:\n");
            client.GetListOfModelNames(modelsInfo, sessionID, "", "");
            printf("Return status: %s\n", modelsInfo.status.data());
            printf("Comments: %s\nModels:\n", modelsInfo.report.data());
            for (std::vector<VELaSSCoSM::FullyQualifiedModelName>::iterator modelIter = modelsInfo.models.begin(); modelIter != modelsInfo.models.end(); modelIter++) {
               printf("%s - %s\n", modelIter->name.data(), modelIter->full_path.data());
            }
         }
      }
      if (strEQL(command, "all") || strEQL(command, "FindModel_DEM_b*")) {
         string DEM_boxModelID;

         printf("\n--->FindModel - \"DEM_b*\"\n");
         client.FindModel(rvOM, sessionID, "DEM_b*", "read");
         printf("Returned modelID: %s\n", rvOM.modelID.data());
         printf("Comments: %s\n", rvOM.report.data());
         DEM_boxModelID = rvOM.modelID;

         rvGetResultFromVerticesID_B rvB;
         vector<int64_t> vertexIDs;

         //printf("\n--->GetResultFromVerticesID - DEM model DEM_box:\n");
         //client.GetResultFromVerticesID(rvB, sessionID, DEM_boxModelID, "", vertexIDs, "Height", 1.0, "geometry");
         //printf("Return status: %s\n", rvB.status.data());
         //printf("Comments: %s\n", rvB.report.data());
      }

      if (strEQL(command, "multi") || strEQL(command, "testListAnalyses")) {
         testListAnalyses(client, sessionID, "telescope");
         //testListAnalyses(client, sessionID, "VELaSSCo_HbaseBasicTest_part_1");
         //testListAnalyses(client, sessionID, "Fluidizedbed");
         //testListAnalyses(client, sessionID, "DEM_box");
      }

      transport->close();
   } catch (TException &tx) {
      printf("ERROR: %s\n", tx.what());
   }
   printf("\n\nEnter a character to stop the program.\n");
   getchar();
   return 0;
}

