#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class Model
{
public:
    Model(ID3D11Device* device);
    ~Model();
    
    void Cleanup();
    void Render(ID3D11DeviceContext* context);

    int GetIndexCount() const;
private:	
    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11Buffer* mIndexBuffer = nullptr;
    int mVertexCount= 0, mIndexCount = 0;
};
