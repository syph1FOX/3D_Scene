#include "Skybox.h"

void Skybox::render(mat4 projection, mat4 model, mat4 view, GLuint mode, vec3 campos, LightsInfo lights)
{
	glDepthMask(GL_FALSE);

	glUseProgram(shaderProgram);

	SetUniformVars(projection, model, view, campos, lights);

	auto loc = glGetUniformLocation(shaderProgram, "skybox");
	glUniform1i(loc, 0);

	if (texID != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	}


	//А дальше всё так же как и в простом случае
	glBindVertexArray(vao);
	if (indicesCount > 0)
		glDrawElements(mode, indicesCount, GL_UNSIGNED_INT, (void*)firstIndexOffset);
	else
		glDrawArrays(mode, 0, vertecesCount);


	glDepthMask(GL_TRUE);
}

void Skybox::render(mat4 projection, mat4 view, vec3 campos, LightsInfo lights)
{
	render(projection, modelMatrix, view, renderMode, campos, lights);
}
