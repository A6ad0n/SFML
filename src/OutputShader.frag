#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform vec3 u_position;
uniform vec2 u_mouse;
uniform float u_FOV;

const float MaxRenderingDist = 10000.0;
const int MaxReflections = 1000;
vec3 light = normalize(vec3(0.85, 0.5, -1.0));

mat2 rotateDir(in float angle)
{
	/*
		@return rotation matrix 2*2 
	*/
	float s = sin(angle);
	float c = cos(angle);
	return mat2(c, -s, s, c);
}

vec2 sphereIntersection(in vec3 origin, in vec3 dir, in float radius) 
{
	/*
		@return vec2

		Check does ray intersect with sphere.
		Sphere assumed to be centered in (0, 0, 0)

		This function gets origin of ray, direction of ray and radius of sphere.
		Then we calculate hitDist - distance from origin to sphere. If it below 0 means there's no intersection.
		It returns intersection.
	*/
	float squareDirLen = dot(dir, dir);
	float hitDist = squareDirLen * radius * radius;

	hitDist += 2.0 * origin.x * dot(origin.yz, dir.yz) * dir.x;
    hitDist += 2.0 * origin.y * origin.z * dir.y * dir.z;
    hitDist -= dot(origin * origin, vec3(dot(dir.yz, dir.yz), dot(dir.xz, dir.xz), dot(dir.xy, dir.xy)));

	if (hitDist < 0.0) 
		return vec2(-1.0);

	hitDist = sqrt(hitDist);
    
    float dist1 = -(hitDist + dot(origin, dir)) / squareDirLen;
    float dist2 = (hitDist - dot(origin, dir)) / squareDirLen;

    return vec2(dist1, dist2);
}

vec2 planeIntersection(in vec3 origin, in vec3 dir, in vec4 position)
{
	/*
		@return vec2

		Check does ray intersect with plane.

		This function gets origin of ray, direction of ray and position of plane.
		Then we calculate intersection and return it.
	*/
	return vec2(-(dot(origin, position.xyz) + position.w) / dot(dir, position.xyz));
}

void createSphere(in vec3 origin, in vec3 dir, in vec3 position, in float radius, in vec4 colorSphere, inout vec2 minIntersection, inout vec3 n, inout vec4 color)
{
	/*
		@return minIntersection and n

		This function creates sphere at position with radius.
	*/
	vec2 intersection = sphereIntersection(origin - position, dir, radius);
	if (intersection.x > 0.0 && intersection.x < minIntersection.x)
	{
		minIntersection = intersection;
		vec3 intersectionPos = dir * intersection.x + origin;
		n = (intersectionPos - position) / radius;
		color = colorSphere;
	}
}

void createPlane(in vec3 origin, in vec3 dir, in vec3 position, inout vec2 minIntersection, inout vec3 n, inout vec4 color)
{
	/*
		@return minIntersection and n

		This function creates plane at position.

	*/
	vec2 intersection = planeIntersection(origin, dir, vec4(position, 1.0));
	if (intersection.x > 0.0 && intersection.x < minIntersection.x)
	{
		minIntersection = intersection;
		n = position;
		color = vec4(0.5, 0.5, 0.5, 0.0);
	}
}

vec3 getSky(in vec3 dir) {
	/*
		@return vec3

		Return sky with sun, sun is done like a specular reflection.
	*/
	vec3 color = vec3(0.0, 0.5, 0.9);
	vec3 sun = vec3(0.95, 0.9, 1.0);
	sun *= max(0.0, pow(dot(dir, light), 32.0));
	color *= max(0.0, dot(light, vec3(0.0, 0.0, -1.0))); //reflection
	return clamp(sun + color, 0.0, 1.0);
}



vec4 rayCast(inout vec3 origin, inout vec3 dir)
{
	/*
		@return vec3

		In this function we construct our scene.
		This function returns color considering light.
		The light source moves along cos and sin functions of time.

		We have diffuse and specular reflection of rays.
		We get max of it, because dot(..., n) can be < 0.
	*/
	vec2 minIntersection = vec2(MaxRenderingDist); //means nearest intersection with any object of scene.
	vec2 intersection;
	vec3 n;
	vec4 color;

	float density = 1.0;

	createSphere(origin, dir, vec3(0.0, 0.0, 0.0), 0.25, vec4(1.0, 0.0, 0.0, 0.2), minIntersection, n, color);
	createSphere(origin, dir, vec3(0.0, -1.0, 0.0), 0.25, vec4(1.0, 0.0, 0.0, 0.4), minIntersection, n, color);
	createSphere(origin, dir, vec3(0.0, -2.0, 0.0), 0.25, vec4(1.0, 0.0, 0.0, 0.6), minIntersection, n, color);
	createSphere(origin, dir, vec3(0.0, -3.0, 0.0), 0.25, vec4(1.0, 0.0, 0.0, 0.8), minIntersection, n, color);
	createSphere(origin, dir, vec3(0.0, -4.0, 0.0), 0.25, vec4(1.0), minIntersection, n, color);

	createPlane(origin, dir, vec3(0.0, 0.0, -1.0), minIntersection, n, color);

	if(minIntersection.x == MaxRenderingDist) //If there's no intersection it will be sky.
		return vec4(-1);

	float diffuse = max(0.0, dot(light, n)) * density;
	float specular = pow(max(0.0, dot(reflect(dir, n), light)), 32.0) * density;
	//color *= mix(diffuse, specular, 0.5);
	vec3 shade = vec3(mix(diffuse, specular, 0.5));
	color.rgb *= mix(shade, vec3(1.0), color.a);
	origin += dir * (minIntersection.x - 0.001);
	dir = reflect(dir, n);
	return color;
}

vec3 rayTrace(in vec3 origin, in vec3 dir)
{
	/*vec3 color = rayCast(origin, dir);
	if (color.x == -1.0)
		return getSky(dir);
	vec3 dirLight = light;
	if (rayCast(origin, dirLight).x != -1.0)
		color *= 0.0;*/

	vec3 color = vec3(dot(light, vec3(0.0, 0.0, -1.0)));
	float reflectivity = 1.0;
	for (int i = 0; i < MaxReflections; ++i)
	{
		vec4 refColor = rayCast(origin, dir);
		if (refColor.x == -1.0) return mix(color, color * getSky(dir), reflectivity);
		vec3 dirLight = light;
		if (rayCast(origin, dirLight).x != -1.0) refColor.rgb *= vec3(refColor.a);
		color *= mix(vec3(1.0), refColor.rgb, reflectivity);
		reflectivity *= refColor.a;
	}
	return color;
}

void main() 
{
	vec2 uv = (gl_TexCoord[0].xy - 0.25) * u_resolution / u_resolution.y;
	vec3 rayOrigin = u_position;
	vec3 rayDir = normalize(vec3(u_FOV, uv));
	rayDir.zx *= rotateDir(-u_mouse.y);
	rayDir.xy *= rotateDir(u_mouse.x);

	vec3 color = rayTrace(rayOrigin, rayDir);

	//Gamma correction
	color.r = pow(color.r, 0.45);
	color.g = pow(color.g, 0.45);
	color.b = pow(color.b, 0.45);

	gl_FragColor = vec4(color, 1.0);
}