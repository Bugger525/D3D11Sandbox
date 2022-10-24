#include "Model.h"
#include <vector>

struct VertexType
{
    XMFLOAT3 Position;
    XMFLOAT4 Color;
};

Model::Model(ID3D11Device* device)
{
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
	mVertexCount = 3;
	mIndexCount = 3;
	
	std::vector<VertexType> vertices(mVertexCount);
    std::vector<unsigned int> indices(mIndexCount);
	
	vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[0].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	
	vertices[1].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	
	vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
}
Model::~Model()
{
    Cleanup();
}
void Model::Cleanup()
{
    
}
void Model::Render(ID3D11DeviceContext context)
{
    
}
int Model::GetIndexCount() const
{
}
