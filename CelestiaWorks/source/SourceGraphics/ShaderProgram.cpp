#include "Graphics/ShaderProgram.h"
#include "SourceGraphics/ShaderProgramImpl.h"

/*
* Tee tähän backend toteutus eli tänne vaa smart pointer siihe pimpl luokkaan.
* Se luokka tarvii temp ShaderObject luokan joka luo sen shaderin. Sitten se tarvitsee myös
*  DescriptorFactory classin, jolla se luo sen cusotom pipelinen ja shader attributet.
* 
*/