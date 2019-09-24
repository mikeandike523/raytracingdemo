#version 330 core
out vec3 color;
uniform float windowWidth;
uniform float windowHeight;
uniform float baseRuler;
uniform sampler2D worldtex;
uniform sampler2D normaltex;
uniform int numTRs;
const float fd = 1000;
const float fog = 700;
const float PI = 3.1415926535897932384626433832795;
const float fovo2=45.0f;
uniform vec3 ppos;
uniform vec3 pang;

float comp(vec3 target, vec3 base);
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}
struct optional_intersection{bool intersected;vec3 pt;};

vec3 grabData(int i){
	return texelFetch(worldtex,ivec2(i,0),0).xyz;
}
vec3 grabNormal(int i){
	return texelFetch(normaltex,ivec2(i,0),0).xyz;
}
float getX(){
	float offset = gl_FragCoord.x-windowWidth/2;
	return offset*1/baseRuler;
}
float getY(){
	float offset = gl_FragCoord.y-windowHeight/2;
	return offset*1/baseRuler;
}
/*
Vector3D intersectPoint(Vector3D rayVector, Vector3D rayPoint, Vector3D planeNormal, Vector3D planePoint) {
	Vector3D diff = rayPoint - planePoint;
	double prod1 = diff.dot(planeNormal);
	double prod2 = rayVector.dot(planeNormal);
	double prod3 = prod1 / prod2;
	return rayPoint - rayVector * prod3;
}*/

optional_intersection intersectPoint(vec3 dir, vec3 n, vec3 o, vec3 rp) {
	vec3 diff = rp-o;
	float prod1 = dot(diff,n);
	float prod2 = dot(dir,n);
	if(abs(prod2)<0.05)
	return optional_intersection(false,vec3(0,0,0));
	float prod3 = prod1 / prod2;
	vec3 result = rp-dir * prod3;

	if(comp(result-rp,dir)<0.05)
	return optional_intersection(false,vec3(0,0,0));
	return optional_intersection(true,result);
}

float angle(vec3 a, vec3 b){
	return acos(dot(a,b)/(length(a)*length(b)));
}
float comp(vec3 target, vec3 base){

	return dot(target,base)/length(base);
}
vec3 proj(vec3 target,vec3 base){
	return comp(target,base)*normalize(base);
}


optional_intersection intersectTriangles(vec3 rayPoint, vec3 ray){
  optional_intersection retvalue=optional_intersection(false,vec3(0,0,0));
  float cutoff = fd;
  for (int i=0;i<numTRs;i++){
	
	vec3 A = grabData(i*3+0);
	vec3 B = grabData(i*3+1);
	vec3 C = grabData(i*3+2);
	//vec3 n = cross(C-A,B-A);
	vec3 n = grabNormal(i);
	optional_intersection isec = intersectPoint(ray,n,A,rayPoint);
	if(!isec.intersected)
		continue;
	else
	{
		vec3 pt = isec.pt;
		if(comp(pt-rayPoint,ray)>fog-5||comp(pt-rayPoint,ray)<0.05)
			continue;


			int failures=0;
		float angle1T = angle(B-A,C-A);
		float angle1M1 =angle(pt-A,B-A);
		float angle1M2 = angle(pt-A,C-A);
		if(angle1M1>angle1T||angle1M2>angle1T)
			failures+=1;

		
		float angle2T = angle(C-B,A-B);
		float angle2M1 =angle(pt-B,C-B);
		float angle2M2 = angle(pt-B,A-B);
		if(angle2M1>angle2T||angle2M2>angle2T)
			failures+=1;

			
			
		float angle3T = angle(A-C,B-C);
		float angle3M1 =angle(pt-C,A-C);
		float angle3M2 = angle(pt-C,B-C);
		if(angle3M1>angle3T||angle3M2>angle3T)
			failures+=1;
		if(failures>0)
		continue;

		if(length(pt-rayPoint)<cutoff){
			cutoff=length(pt-rayPoint);

			retvalue = optional_intersection(true,pt);


			
		}else{
			continue;
		}
	}
	
	

	
  }
  if(cutoff>=fd)
		return optional_intersection(false,vec3(0,0,0));
   return retvalue;
}






void main(){
  float x = getX();
  float y = getY();
  vec3 ray = (rotationMatrix(vec3(0,0,1),-pang.z)*rotationMatrix(vec3(0,1,0),pang.x-PI/2)*rotationMatrix(vec3(1,0,0),pang.y)*vec4(x,y,fd,1)).xyz;
	optional_intersection worldresult = intersectTriangles(ppos,ray);
  if(worldresult.intersected==false)
		discard;
		else{


			optional_intersection lightresult = intersectTriangles(worldresult.pt,vec3(0,50,0)-worldresult.pt);
			if(!lightresult.intersected)
			color  = vec3(1,0,0);
			else
			color=vec3(0,0,1);
		}
		
}