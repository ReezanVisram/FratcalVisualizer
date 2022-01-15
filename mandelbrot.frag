#version 460 core
in vec4 gl_FragCoord;

uniform vec2 WindowSize;
uniform vec3 Controls;

out vec4 FragColor;


#define MAX_ITERATIONS 5000
#define ESCAPE_RADIUS 4

float getNumIterations() {
	float realComponent = ((gl_FragCoord.x / WindowSize.x - (WindowSize.y / WindowSize.x)) * Controls.z + Controls.x) * 3.0;
	
	float imaginaryComponent = ((gl_FragCoord.y / WindowSize.y - 0.5f) * Controls.z + Controls.y) * 2.0;

	float iterations = 0;
	float REAL_COMPONENT = realComponent;
	float IMAG_COMPONENT = imaginaryComponent;

	while (iterations  < MAX_ITERATIONS) {
		float tempReal = realComponent; 

		realComponent = (realComponent * realComponent - imaginaryComponent * imaginaryComponent) + REAL_COMPONENT;
		imaginaryComponent = (2.0 * tempReal * imaginaryComponent) + IMAG_COMPONENT;

		float modulusSquared = realComponent * realComponent + imaginaryComponent * imaginaryComponent;

		if (modulusSquared > 4.0f) {
			break;
		}

		++iterations;
	}

	return iterations;
}

float iterate(vec2 p) {
	vec2 z = vec2(0), c = p;

	float iterations;

	for (iterations = 0; iterations < MAX_ITERATIONS; iterations++) {
		z = mat2(z, -z.y, z.x) * z + c;
		if (dot(z, z) > ESCAPE_RADIUS*ESCAPE_RADIUS) {
			break;
		}
	}

	if (p.y < 0) {
		return iterations;
	}

	return iterations - log(log(dot(z, z)) / log(ESCAPE_RADIUS)) / log(2);
}

vec3 palette(in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d) {
	return a + b*cos(6.28318*(c*t + d));

}

vec4 newReturnColor() {
	vec2 screenRes = WindowSize.xy;
	vec2 fragCoord = gl_FragCoord.xy;

	vec2 uv = (1.2 * (2 * fragCoord - screenRes - 1) / screenRes.y - vec2(0.4, 0)) * Controls.z + Controls.xy;

	float numIterations = iterate(uv) / MAX_ITERATIONS;

	vec3 color = palette(fract(numIterations + 0.5), vec3(0, 0.5, 0.5), vec3(0, 0.5, 0.5), vec3(0, 0.5, 0.33), vec3(0, 0.5, 0.66));


	return vec4(numIterations == 1 ? vec3(0) : color, 1.0);
}



vec4 returnColor() {
	float iterations = getNumIterations() / MAX_ITERATIONS;

	vec3 color = palette(fract(iterations + 0.5), vec3(0, 0.5, 0.5), vec3(0, 0.5, 0.5), vec3(0, 0.5, 0.33), vec3(0, 0.5, 0.66));

	return vec4(iterations == 1 ? vec3(0) : color, 1.0);
}


void main() {
	FragColor = newReturnColor();
}

