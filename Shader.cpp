#include "Shader.h"

#include <sstream>
#include "ErrorHandler.h"

Shader::Shader(const char* filepath1, const char* filepath2) {
	std::string content1 = "", content2 = "",line="";
	std::ifstream wah(filepath1, std::ios::in);
	std::ifstream wa(filepath2, std::ios::in);
	unsigned int vertexshader=glCreateShader(GL_VERTEX_SHADER), fragmentshader=glCreateShader(GL_FRAGMENT_SHADER);
	while (!wah.eof()) {
		std::getline(wah, line);
		content1.append(line + "\n");
	}
	line = "";
	while (!wa.eof()) {
		std::getline(wa, line);
		content2.append(line + "\n");
	}
	const char* a = content1.c_str();
	const char* aa = content2.c_str();
	glShaderSource(vertexshader, 1, &a, NULL);
	glCompileShader(vertexshader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &aa, NULL);
	glCompileShader(fragmentshader);
	
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	id = glCreateProgram();
	glAttachShader(id, vertexshader);
	glAttachShader(id, fragmentshader);
	glLinkProgram(id);
	
	std::cout << id << "\n";
}

Shader::Shader() {
	
}

Shader::~Shader() {
	std::cout << "fewafawefaw " << id << std::endl;
	glDeleteProgram(id);
}

void Shader::createFromSource(const std::string& source) {
	std::string content="";
	std::string line;
	std::ifstream file(source, std::ios::in);
	while (!file.eof()) {
		std::getline(file, line);
		content.append(line + "\n");
	}
	std::istringstream iss(content);
	std::stringstream ss[2];
	int type = -1;
	for (std::string line; std::getline(iss, line); ) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) { type = 0; }
			else if (line.find("fragment") != std::string::npos) { type = 1; }
		}
		else {
			if (type == -1) std::cout << "[ERROR] no specifying shader type found" << std::endl;
			ss[type] << line << '\n';
		}
	}
	//std::cout << ss[0].str() << std::endl << ss[1].str() << std::endl;
	createFromSource(ss[0].str(),ss[1].str());
}

void Shader::createFromSource(const std::string& vertexsrc, const std::string& fragsrc) {
	std::string content1 = vertexsrc, content2 = fragsrc;
	unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER), fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* a = content1.c_str();
	const char* aa = content2.c_str();
	glShaderSource(vertexshader, 1, &a, NULL);
	glCompileShader(vertexshader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &aa, NULL);
	glCompileShader(fragmentshader);

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	id = glCreateProgram();
	glAttachShader(id, vertexshader);
	glAttachShader(id, fragmentshader);
	glLinkProgram(id);

	std::cout << id << "\n";
}

void Shader::use() {
	GLCall(glUseProgram(id));
	//std::cout << "use " << id << std::endl;
	
	/*GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "OpenGL Error after glUseProgram(): " << err << " " << id << std::endl;
	}*/

}

unsigned int Shader::getId() {
	return id;
}

void Shader::SetUniformMat4f(std::string uniformName, const glm::mat4 &matrix) {
	glUniformMatrix4fv(glGetUniformLocation(id, uniformName.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniform1i(std::string uniformName, const int& value) {
	glUniform1i(glGetUniformLocation(id, uniformName.c_str()), value);
}

void Shader::SetUniform1f(std::string uniformName, const float &value) {
	glUniform1f(glGetUniformLocation(id, uniformName.c_str()), value);
}

void Shader::SetUniformVec4(std::string uniformName, const glm::vec4 &vector) {
	glUniform4fv(glGetUniformLocation(id, uniformName.c_str()), 1, &vector[0]);
}



void Shader::SetUniformVec3(std::string uniformName, const glm::vec3& vector) {
	glUniform3fv(glGetUniformLocation(id, uniformName.c_str()), 1, glm::value_ptr(vector));
}
