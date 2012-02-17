/**
 *  @example oglplus/024_extruded_torus.cpp
 *  @brief Shows how to draw a torus with extruded surface
 *
 *  @image html 024_extruded_torus.png
 *
 *  Copyright 2008-2012 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/torus.hpp>

#include <cmath>

#include "example.hpp"

namespace oglplus {

class TorusExample : public Example
{
private:
	shapes::Torus make_torus;
	shapes::DrawingInstructions torus_instr;
	shapes::Torus::IndexArray torus_indices;

	// wrapper around the current OpenGL context
	Context gl;

	VertexShader vs;
	GeometryShader gs;
	FragmentShader face_fs, frame_fs;
	Program transf_prog, face_prog, frame_prog;
	ProgramPipeline face_pp, frame_pp;

	// A vertex array object for the rendered torus
	VertexArray torus;

	// VBOs for the torus's vertices, normals and st coordinates
	Buffer verts, normals, texcoords;
public:
	TorusExample(void)
	 : make_torus(1.0, 0.5, 18, 36)
	 , torus_instr(make_torus.Instructions())
	 , torus_indices(make_torus.Indices())
	 , vs("Vertex")
	 , gs("Geometry")
	 , face_fs("Face fragment")
	 , frame_fs("Frame fragment")
	 , transf_prog("Transformation")
	 , face_prog("Face")
	 , frame_prog("Frame")
	{
		vs.Source(
			"#version 330\n"
			"uniform mat4 ModelMatrix;"
			"in vec4 Position;"
			"in vec3 Normal;"
			"in vec2 TexCoord;"
			"out gl_PerVertex {"
			"	vec4 gl_Position;"
			"};"
			"out vec3 vertNormal;"
			"out vec2 vertTexCoord;"
			"void main(void)"
			"{"
			"	gl_Position = ModelMatrix * Position;"
			"	vertNormal = (ModelMatrix*vec4(Normal,0.0)).xyz;"
			"	vertTexCoord = TexCoord;"
			"}"
		);
		vs.Compile();

		gs.Source(
			"#version 330\n"
			"layout(triangles) in;"
			"layout(triangle_strip, max_vertices = 15) out;"
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform vec3 LightPos;"
			"uniform float Time;"
			"in gl_PerVertex {"
			"	vec4 gl_Position;"
			"} gl_in[];"
			"in vec3 vertNormal[];"
			"in vec2 vertTexCoord[];"
			"out gl_PerVertex {"
			"	vec4 gl_Position;"
			"};"
			"out vec3 geomNormal;"
			"out vec3 geomLight;"
			"out float geomGlow;"
			"flat out int geomTop;"
			"void main(void)"
			"{"
			"	vec3 FaceNormal = normalize("
			"		vertNormal[0]+"
			"		vertNormal[1]+"
			"		vertNormal[2] "
			"	);"
			"	vec2 FaceCoord = 0.33333 * ("
			"		vertTexCoord[0]+"
			"		vertTexCoord[1]+"
			"		vertTexCoord[2] "
			"	);"
			"	float Offs = (sin((FaceCoord.s + Time/10.0)* 3.14 * 2.0 * 10)*0.5 + 0.5)*0.4;"
			"	Offs *= cos(FaceCoord.t * 3.1415 * 2.0)*0.5 + 0.51;"

			"	vec3 pos[3], norm[3];"
			"	for(int i=0; i!=3; ++i)"
			"		pos[i] = gl_in[i].gl_Position.xyz;"
			"	for(int i=0; i!=3; ++i)"
			"		norm[i] = cross("
			"			FaceNormal, "
			"			normalize(pos[(i+1)%3] - pos[i])"
			"		);"
			"	vec3 pofs = FaceNormal * Offs;"

			"	geomTop = 0;"
			"	for(int i=0; i!=3; ++i)"
			"	{"
			"		geomNormal = norm[i];"
			"		for(int j=0; j!=2; ++j)"
			"		{"
			"			vec3 tpos = pos[(i+j)%3];"
			"			geomLight = LightPos-tpos;"
			"			geomGlow = 1.0;"
			"			gl_Position = "
			"				ProjectionMatrix *"
			"				CameraMatrix *"
			"				vec4(tpos, 1.0);"
			"			EmitVertex();"
			"			geomGlow = 0.7;"
			"			geomLight = LightPos-tpos+pofs;"
			"			gl_Position = "
			"				ProjectionMatrix *"
			"				CameraMatrix *"
			"				vec4(tpos + pofs, 1.0);"
			"			EmitVertex();"
			"		}"
			"		EndPrimitive();"
			"	}"

			"	geomGlow = 0.0;"
			"	geomTop = 1;"
			"	for(int i=0; i!=3; ++i)"
			"	{"
			"		geomLight = LightPos - (pos[i]+pofs);"
			"		geomNormal = vertNormal[i];"
			"		gl_Position = "
			"			ProjectionMatrix *"
			"			CameraMatrix *"
			"			vec4(pos[i] + pofs, 1.0);"
			"		EmitVertex();"
			"	}"
			"	EndPrimitive();"
			"}"
		);
		gs.Compile();

		transf_prog.AttachShader(vs);
		transf_prog.AttachShader(gs);
		transf_prog.MakeSeparable();
		transf_prog.Link();
		transf_prog.Use();

		ProgramUniform<Vec3f>(transf_prog, "LightPos").Set(4, 4, -8);

		torus.Bind();
		verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Positions(data);
			Buffer::Data(Buffer::Target::Array, data);

			VertexAttribArray attr(transf_prog, "Position");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.Normals(data);
			Buffer::Data(Buffer::Target::Array, data);

			VertexAttribArray attr(transf_prog, "Normal");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		texcoords.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_torus.TexCoordinates(data);
			Buffer::Data(Buffer::Target::Array, data);

			VertexAttribArray attr(transf_prog, "TexCoord");
			attr.Setup(n_per_vertex, DataType::Float);
			attr.Enable();
		}

		face_fs.Source(
			"#version 330\n"
			"in vec3 geomNormal;"
			"in vec3 geomLight;"
			"in float geomGlow;"
			"flat in int geomTop;"
			"uniform vec3 TopColor, SideColor;"
			"const vec3 LightColor = vec3(1.0, 1.0, 1.0);"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	float d = max(dot("
			"		normalize(geomLight),"
			"		normalize(geomNormal)"
			"	), 0.0);"
			"	vec3 color;"
			"	if(geomTop != 0)"
			"	{"
			"		color = TopColor * d +"
			"			LightColor * pow(d, 8.0);"
			"	}"
			"	else"
			"	{"
			"		color = SideColor * geomGlow +"
			"			LightColor *"
			"			pow(d, 2.0) * 0.2;"
			"	}"
			"	fragColor = vec4(color, 1.0);"
			"}"
		);
		face_fs.Compile();

		face_prog.AttachShader(face_fs);
		face_prog.MakeSeparable();
		face_prog.Link();

		ProgramUniform<Vec3f>(face_prog, "TopColor").Set(0.2, 0.2, 0.2);
		ProgramUniform<Vec3f>(face_prog, "SideColor").Set(0.9, 0.9, 0.2);

		face_pp.Bind();
		face_prog.Use();
		face_pp.UseStages(transf_prog).Vertex().Geometry();
		face_pp.UseStages(face_prog).Fragment();


		frame_fs.Source(
			"#version 330\n"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(0.2, 0.1, 0.0, 1.0);"
			"}"
		);
		frame_fs.Compile();

		frame_prog.AttachShader(frame_fs);
		frame_prog.MakeSeparable();
		frame_prog.Link();

		frame_pp.Bind();
		frame_prog.Use();
		frame_pp.UseStages(transf_prog).Vertex().Geometry();
		frame_pp.UseStages(frame_prog).Fragment();

		ProgramPipeline::Unbind();
		Program::UseNone();

		gl.ClearColor(0.7f, 0.6f, 0.5f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Enable(Capability::DepthTest);
		gl.DepthFunc(CompareFn::Less);
		gl.FrontFace(make_torus.FaceWinding());
	}

	void Reshape(size_t width, size_t height)
	{
		gl.Viewport(width, height);
		auto projection = CamMatrixf::Perspective(
			Degrees(48),
			double(width)/height,
			1, 100
		);
		ProgramUniform<Mat4f>(
			transf_prog,
			"ProjectionMatrix"
		).Set(projection);
	}

	void Render(double time)
	{
		gl.Clear().ColorBuffer().DepthBuffer();
		//
		auto camera = CamMatrixf::Orbiting(
			Vec3f(),
			3.5,
			Degrees(time * 35),
			Degrees(SineWave(time / 30.0) * 60)
		);

		auto model =
			ModelMatrixf::RotationY(FullCircles(time * 0.25)) *
			ModelMatrixf::RotationX(FullCircles(time * 0.33));

		ProgramUniform<Mat4f>(transf_prog, "CameraMatrix").Set(camera);
		ProgramUniform<Mat4f>(transf_prog, "ModelMatrix").Set(model);
		ProgramUniform<GLfloat>(transf_prog, "Time").Set(time);

		face_pp.Bind();
		gl.PolygonMode(PolygonMode::Fill);
		torus_instr.Draw(torus_indices);

		frame_pp.Bind();
		gl.PolygonMode(PolygonMode::Line);
		torus_instr.Draw(torus_indices);
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}
};

std::unique_ptr<Example> makeExample(const ExampleParams& params)
{
	return std::unique_ptr<Example>(new TorusExample);
}

} // namespace oglplus
