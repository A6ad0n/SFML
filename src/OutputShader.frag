uniform vec2 u_resolution;


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


void main() 
{
	vec2 uv = (gl_TexCoord[0].xy - 0.5) * u_resolution / u_resolution.y;
	vec3 rayOrigin = vec3(-10.0, 3.0, 2.5);
	vec3 rayDir = normalize(vec3(1.0, uv));
	float T = sphereIntersection(rayOrigin, rayDir, 1.0);
	if (T >= 0.0)
		gl_FragColor = vec4(normalize(rayDir * T + rayOrigin) * 0.5 + 0.5, 1.0);
	else
		gl_FragColor = vec4(vec3(0.6), 1.0);
}