SET SHADERS_FOLDER_PATH = C:/dev/CelestiaWorks/CelestiaWorks/source/shaders




C:\VulkanSDK\1.3.243.0\Bin\glslangValidator.exe -V -o batchRenderFrag.h --vn batchFragmentShader batchRender.frag

C:\VulkanSDK\1.3.243.0\Bin\glslangValidator.exe -V -o batchRenderVert.h --vn batchVertexShader batchRender.vert

REM C:\VulkanSDK\1.3.243.0\Bin\glslangValidator.exe -V --vn defaultRenderFrag defaultRenderFrag.frag

REM C:\VulkanSDK\1.3.243.0\Bin\glslangValidator.exe --vn defaultRenderFrag %SHADERS_FOLDER_PATH%/defaultRenderFrag.frag -o defaultRenderFrag.spv.h


REM C:\VulkanSDK\1.3.243.0\Bin\glslangValidator.exe -V --vn %SHADERS_FOLDER_PATH%/defaultRenderVert.vert -o defaultRenderVert.spv.h
pause