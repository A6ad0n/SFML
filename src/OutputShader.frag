uniform vec2 u_resolution;
uniform float u_time;


float sphereIntersection(in vec3 origin, in vec3 dir, float radius) 
{
	/*
		@return float

		Check does ray intersect with sphere.
		Sphere assumed to be centered in (0, 0, 0)

		This function gets origin of ray, direction of ray and radius of sphere.
		Then we calculate hitDist - distance from origin to sphere. If it below 0 means there's no intersection.
		It returns nearest point of intersection
	*/
	float squareDirLen = dot(dir, dir);
	float hitDist = squareDirLen * radius * radius;

	hitDist += 2.0 * origin.x * dot(origin.yz, dir.yz) * dir.x;
    hitDist += 2.0 * origin.y * origin.z * dir.y * dir.z;
    hitDist -= dot(origin * origin, vec3(dot(dir.yz, dir.yz), dot(dir.xz, dir.xz), dot(dir.xy, dir.xy)));

	if (hitDist < 0.0) 
		return -1.0;

	hitDist = sqrt(hitDist);
    
    float dist1 = (hitDist - dot(origin, dir)) / squareDirLen;
    float dist2 = -(hitDist + dot(origin, dir)) / squareDirLen;

    if (dist1 >= 0.0 && dist2 >= 0.0)
        return min(dist1, dist2);
    else
        return max(dist1, dist2);
}

float rayCast(float dist, in vec3 dir, in vec3 origin)
{
	/*
		@return float

		This function returns color considering light.
		The light source moves along cos and sin functions of time.
		We have diffuse and specular reflection of rays.
		We get max of it, because dot(..., n) can be < 0.
	*/
	float density = 0.5;
	vec3 pos = dir * dist + origin; //position of intersection
	vec3 n = pos;
	vec3 light = normalize(vec3(cos(u_time), 0.75, sin(u_time))); //direction of light
	float diffuse = max(0.0, dot(light, n)) * density;
	float specular = pow(max(0.0, dot(reflect(dir, n), light)), 32.0) * density;
	return diffuse + specular;
}

void main() 
{
	vec2 uv = (gl_TexCoord[0].xy - 0.5) * u_resolution / u_resolution.y;
	vec3 rayOrigin = vec3(-5.0, 0.0, 0.0); //Cam position
	vec3 rayDir = normalize(vec3(1.0, uv)); //Direction of rays
	float T = sphereIntersection(rayOrigin, rayDir, 1.0); //Distance from origin to sphere
	vec3 light = vec3(rayCast(T, rayDir, rayOrigin));
	vec3 color;
	if (T < 0.0)
		color = vec3(0.0);
	else
		color = light;
	gl_FragColor = vec4(color, 1.0);
}