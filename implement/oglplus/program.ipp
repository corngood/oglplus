/**
 *  @file oglplus/program.ipp
 *  @brief Implementation of Program
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

namespace oglplus {

#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_4_3

OGLPLUS_LIB_FUNC
GLenum ProgramResource::ReferencedByProperty(ShaderType shader_type) const
{
	if(shader_type == ShaderType::Vertex)
	{
		return GL_REFERENCED_BY_VERTEX_SHADER;
	}
	if(shader_type == ShaderType::TessControl)
	{
		return GL_REFERENCED_BY_TESS_CONTROL_SHADER;
	}
	if(shader_type == ShaderType::TessEvaluation)
	{
		return GL_REFERENCED_BY_TESS_EVALUATION_SHADER;
	}
	if(shader_type == ShaderType::Geometry)
	{
		return GL_REFERENCED_BY_GEOMETRY_SHADER;
	}
	if(shader_type == ShaderType::Fragment)
	{
		return GL_REFERENCED_BY_FRAGMENT_SHADER;
	}
	if(shader_type == ShaderType::Compute)
	{
		return GL_REFERENCED_BY_COMPUTE_SHADER;
	}
	return GL_NONE;
}

#endif // GL_VERSION_4_3

OGLPLUS_LIB_FUNC
void ProgramOps::HandleLinkError(void) const
{
	HandleBuildError<LinkError>(
		GetInfoLog(),
		OGLPLUS_OBJECT_ERROR_INFO(
			LinkProgram,
			Program,
			nullptr,
			_name
		)
	);
}

OGLPLUS_LIB_FUNC
void ProgramOps::TransformFeedbackVaryings(
	const std::vector<String>& varyings,
	TransformFeedbackMode mode
) const
{
	std::vector<const GLchar*> tmp(varyings.size());
	auto i = varyings.begin(), e = varyings.end();
	auto t = tmp.begin();
	while(i != e)
	{
		assert(t != tmp.end());
		*t = i->c_str();
		++i;
		++t;
	}
	OGLPLUS_GLFUNC(TransformFeedbackVaryings)(
		_name,
		GLsizei(tmp.size()),
		tmp.data(),
		GLenum(mode)
	);
	OGLPLUS_CHECK(OGLPLUS_OBJECT_ERROR_INFO(
		TransformFeedbackVaryings,
		Program,
		nullptr,
		_name
	));
}

#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_4_1 || GL_ARB_get_program_binary
OGLPLUS_LIB_FUNC
void ProgramOps::GetBinary(std::vector<GLubyte>& binary, GLenum& format) const
{
	assert(_name != 0);
	GLint size = GetIntParam(GL_PROGRAM_BINARY_LENGTH);
	if(size > 0)
	{
		GLsizei len = 0;
		binary.resize(size);
		OGLPLUS_GLFUNC(GetProgramBinary)(
			_name,
			size,
			&len,
			&format,
			binary.data()
		);
		OGLPLUS_CHECK(OGLPLUS_OBJECT_ERROR_INFO(
			GetProgramBinary,
			Program,
			nullptr,
			_name
		));
	}
}
#endif

} // namespace oglplus

