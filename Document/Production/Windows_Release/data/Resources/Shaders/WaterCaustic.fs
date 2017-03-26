precision highp float;

varying vec2 v_uv;

uniform sampler2D u_texBase;			//base
uniform sampler2D u_texDisp;			//displament map

uniform float u_time;

void main()
{	
	vec2 disp = texture2D(u_texDisp, vec2(v_uv.x * 2.0 + u_time, v_uv.y * 2.0 )).rg;
	
	vec2 offset_base = (2.0 * disp - 1.0) * 0.04;	//calculate depth base on u
	vec2 base_uv = v_uv + offset_base;
	vec4 baseColor = texture2D(u_texBase, base_uv * 2.0);	
			
	gl_FragColor = baseColor;
}