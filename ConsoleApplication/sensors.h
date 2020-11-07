#pragma once

enum class fixedwaterlevelsensorstate_t {
	undetermined,
	wet,
	dry
};

enum class waterlevelsensortype_t {
	unknown,
	optical,
	capacitive,
	resistive
};

enum class waterlevelsensorsubtype_t {
	unknown,
	fixed,
	floating
};