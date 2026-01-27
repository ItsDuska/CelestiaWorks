#include "Graphics/ShaderProgram.hpp"
#include "Graphics/ShaderTypes.hpp"
#include "ShaderProgramImpl.hpp"

/*
 * Tee tähän backend toteutus eli tänne vaa smart pointer siihe pimpl luokkaan.
 * Se luokka tarvii temp ShaderObject luokan joka luo sen shaderin. Sitten se tarvitsee myös
 *  DescriptorFactory classin, jolla se luo sen cusotom pipelinen ja shader attributet.
 *
 */
void celestia::ShaderProgram::loadFromFile(const std::string& filepath, ShaderType stage)
{

}
