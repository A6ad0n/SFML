void main() 
{
	vec2 uv = gl_TexCoord[0].xy;
	gl_FragColor = vec4(uv, 2.0, 5.0);
}