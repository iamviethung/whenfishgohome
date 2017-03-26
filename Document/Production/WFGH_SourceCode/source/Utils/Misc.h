#pragma once

#include "stdafx.h"
#include "Vertex.h"

#define BUFFER_SIZE 256

#define VERTEX_DATA_FORMAT "%d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];"
#define INDEX_DATA_FORMAT "   %d.    %d,    %d,    %d"

bool readModelFile(char* path, Vertex *&vertices, int &vertices_size, GLushort *&indices, int &indices_size)
{
	FILE *f;
	f = fopen(path, "rb");

	if (!f)
		return false;

	char *data_buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);

	// read number of vertices
	int numOfVertices;

	if (!fgets(data_buffer, BUFFER_SIZE, f))
		return false;	

	sscanf(data_buffer, "NrVertices: %d", &numOfVertices);
	vertices_size = numOfVertices;
	
	//read vertices data
	Vertex *model_vertices = (Vertex*) malloc(sizeof(Vertex)*numOfVertices);
	int index;

	for (int i = 0; i < numOfVertices; i++)
	{
		if (!fgets(data_buffer, BUFFER_SIZE, f))
			return false;

		sscanf(data_buffer, VERTEX_DATA_FORMAT,
			&index,
			&model_vertices[i].pos.x,
			&model_vertices[i].pos.y,
			&model_vertices[i].pos.z,
			&model_vertices[i].norm.x,
			&model_vertices[i].norm.y,
			&model_vertices[i].norm.z,
			&model_vertices[i].binorm.x,
			&model_vertices[i].binorm.y,
			&model_vertices[i].binorm.z,
			&model_vertices[i].tgt.x,
			&model_vertices[i].tgt.y,
			&model_vertices[i].tgt.z,
			&model_vertices[i].uv.x,
			&model_vertices[i].uv.y
			);
	}

	vertices = model_vertices;

	//read num of indices
	int numOfIndices;
	if (!fgets(data_buffer, BUFFER_SIZE, f))
		return false;

	sscanf(data_buffer, "NrIndices: %d", &numOfIndices);
	indices_size = numOfIndices;

	//read indices data
	GLushort *model_indices = (GLushort*) malloc(sizeof(GLushort)*numOfIndices);

	for (int i = 0; i < numOfIndices; i += 3)
	{
		if (!fgets(data_buffer, BUFFER_SIZE, f))
			return false;

		sscanf(data_buffer, INDEX_DATA_FORMAT, 
						&index,
						&model_indices[i],
						&model_indices[i+1],
						&model_indices[i+2]
						);
	}

	indices = model_indices;

	//clean up
	fclose(f);

	if (data_buffer)
		free(data_buffer);

	return true;
	
}

//NFGVertex * readNFGFile_BadWay_LOL(char* path)
//{
//	FILE *f;
//	f = fopen(path, "rb");
//
//	if (!f)
//		return 0;
//
//	//read all file to memory
//	int file_size = -1;
//	fseek(f, 0, SEEK_END);
//	file_size = ftell(f);
//	rewind(f);
//
//	char *buffer = (char*) malloc(sizeof(char)*file_size + 1);
//
//	int result = fread(buffer, 1, file_size, f);
//	buffer[file_size] = '\0';
//
//	fclose(f);
//
//	int nVertices = -10;
//	char *line_buffer = (char*) malloc(sizeof(char)*BUFFER_SIZE);
//	int tmp;
//
//	float vertex_data[VERTEX_ATTRIB_SIZE];
//	float a;
//
//	//====================
//	char *static_buffer = "   0. pos:[0.134000, 1.020300, -0.083900]; norm:[0.662725, 0.317712, -0.678126]; binorm:[0.014559, 0.899903, 0.435847]; tgt:[-0.748721, 0.298719, -0.591763]; uv:[0.611900, 0.886700];";
//	char *tmp_buffer = (char*) malloc(strlen(static_buffer));
//	strcpy(tmp_buffer, static_buffer);
//
//	char *tmp_buffer1;
//	int pos;
//
//	tmp_buffer = strchr(tmp_buffer, '[') + 1;
//	tmp_buffer1 = strchr(tmp_buffer, ']');
//	*tmp_buffer1 = '\0';
//
//	sscanf(tmp_buffer, "%f, %f, %f", &vertex_data[0],
//								  &vertex_data[1],
//								  &vertex_data[2]);
//
//	//====================
//
//	//analyze file
//	while(buffer)
//	{
//		char * nextLine = strchr(buffer, '\n');
//		if (nextLine) 
//			*nextLine = '\0';  // temporarily terminate the current line
//		
//		printf("curLine=[%s]\n", buffer);
//
//		//====
//		if (nVertices == -10)
//		{
//			sscanf(buffer, "%s %i", line_buffer, &nVertices);
//		}
//		else if(nVertices > 0)		//read all vertices
//		{
//			/*sscanf(buffer, "%i %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
//							&tmp,
//							&vertex_data[0],
//							&vertex_data[1],
//							&vertex_data[2],
//							&vertex_data[3],
//							&vertex_data[4],
//							&vertex_data[5],
//							&vertex_data[6],
//							&vertex_data[7],
//							&vertex_data[8],
//							&vertex_data[9],
//							&vertex_data[10],
//							&vertex_data[11],
//							&vertex_data[12],
//							&vertex_data[13]);*/
//
//			// read pos
//			line_buffer = strchr(buffer, '[') + 1;
//			tmp_buffer = strchr(line_buffer, ']');
//			*tmp_buffer = '\0';
//
//			sscanf(line_buffer, "%f, %f, %f",	&vertex_data[0],
//												&vertex_data[1],
//												&vertex_data[2]);
//			*tmp_buffer = ']';
//			line_buffer = tmp_buffer + 1;
//
//			// read norm
//			line_buffer = strchr(line_buffer, '[') + 1;
//			tmp_buffer = strchr(line_buffer, ']');
//			*tmp_buffer = '\0';
//
//			sscanf(line_buffer, "%f, %f, %f",	&vertex_data[3],
//												&vertex_data[4],
//												&vertex_data[5]);
//			*tmp_buffer = ']';
//			line_buffer = tmp_buffer + 1;
//
//			// read binorm
//			line_buffer = strchr(line_buffer, '[') + 1;
//			tmp_buffer = strchr(line_buffer, ']');
//			*tmp_buffer = '\0';
//
//			sscanf(line_buffer, "%f, %f, %f",	&vertex_data[6],
//												&vertex_data[7],
//												&vertex_data[8]);
//			*tmp_buffer = ']';
//			line_buffer = tmp_buffer + 1;
//
//			// read tgt
//			line_buffer = strchr(line_buffer, '[') + 1;
//			tmp_buffer = strchr(line_buffer, ']');
//			*tmp_buffer = '\0';
//
//			sscanf(line_buffer, "%f, %f, %f",	&vertex_data[9],
//												&vertex_data[10],
//												&vertex_data[11]);
//			*tmp_buffer = ']';
//			line_buffer = tmp_buffer + 1;
//
//			// read uv
//			line_buffer = strchr(line_buffer, '[') + 1;
//			tmp_buffer = strchr(line_buffer, ']');
//			*tmp_buffer = '\0';
//
//			sscanf(line_buffer, "%f, %f",		&vertex_data[12],
//												&vertex_data[13]);
//			*tmp_buffer = ']';
//			
//			//decrease num of vetex
//			nVertices--;
//		}
//		//====
//
//		if (nextLine) 
//			*nextLine = '\n';  // then restore newline-char, just to be tidy    
//			
//		buffer = nextLine ? (nextLine+1) : NULL;
//	}
//
//	free(buffer);
//	free(line_buffer);
//
//	return 0;
//}