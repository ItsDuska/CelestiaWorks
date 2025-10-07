#include "Graphics/ShaderProgram.hpp"
#include "SourceGraphics/ShaderProgramImpl.hpp"

/*
 * Tee t�h�n backend toteutus eli t�nne vaa smart pointer siihe pimpl luokkaan.
 * Se luokka tarvii temp ShaderObject luokan joka luo sen shaderin. Sitten se tarvitsee my�s
 *  DescriptorFactory classin, jolla se luo sen cusotom pipelinen ja shader attributet.
 *
 */