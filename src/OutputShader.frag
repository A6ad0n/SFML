#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;
uniform float u_time;
uniform vec3 u_position;
uniform vec3 u_mouse;

const float MaxRenderingDist = 10000.0;

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

void createSphere(in vec3 origin, in vec3 dir, in vec3 position, in float radius, inout vec2 minIntersection, inout vec3 n)
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
	}
}

void createPlane(in vec3 origin, in vec3 dir, in vec3 position, inout vec2 minIntersection, inout vec3 n)
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
	}
}



vec3 rayCast(in vec3 origin, in vec3 dir)
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

	float density = 0.5;

	createSphere(origin, dir, vec3(0.0, 0.0, 0.0), 0.5, minIntersection, n);
	createSphere(origin, dir, vec3(0.0, -1.0, 0.0), 0.5, minIntersection, n);
	createSphere(origin, dir, vec3(0.0, 1.0, 0.0), 0.5, minIntersection, n);
	createSphere(origin, dir, vec3(0.0, 0.0, -1.0), 0.5, minIntersection, n);
	createSphere(origin, dir, vec3(0.0, 0.0, -2.0), 0.5, minIntersection, n);

	createPlane(origin, dir, vec3(0.0, 0.0, -1.0), minIntersection, n);

	if(minIntersection.x == MaxRenderingDist)
		return vec3(0.0, 0.5, 0.9);

	vec3 light = normalize(vec3(sin(u_time * 0.3), 0.75, cos(u_time * 0.3))); //direction of light
	float diffuse = max(0.0, dot(light, n)) * density;
	float specular = pow(max(0.0, dot(reflect(dir, n), light)), 32.0) * density;
	vec3 color = vec3(diffuse + specular);
	return color;
}

void main() 
{
	vec2 uv = (gl_TexCoord[0].xy - 0.25) * u_resolution / u_resolution.y;
	vec3 rayOrigin = u_position; //Cam position
	vec3 rayDir = normalize(vec3(1.0, uv)); //Direction of rays
	//rayDir.zx *= rotateDir(-u_mouse.y);
	//rayDir.xy *= rotateDir(u_mouse.x);
	vec3 color = rayCast(rayOrigin, rayDir);
	gl_FragColor = vec4(color, 1.0);
}