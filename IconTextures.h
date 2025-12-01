#include <glm/glm.hpp>

inline glm::vec4 GetIcon(const glm::vec4& uvrect, const glm::vec2& atlasSize) {
	return glm::vec4(uvrect.x/atlasSize.x, 0, 16/atlasSize.x, 16/atlasSize.y);
}

inline glm::vec4 GetUV(int idx) {
	return glm::vec4(idx*16,0, 16, 16);
}
