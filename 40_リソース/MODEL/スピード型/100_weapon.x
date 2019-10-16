xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 305;
 -1.98000;-1.98000;-30.99165;,
 0.00000;-2.97000;-30.99166;,
 0.00000;-2.97000;-30.06808;,
 -1.98000;1.98000;-30.99164;,
 -1.98000;1.98000;-30.06807;,
 0.00000;2.97000;-30.06808;,
 -2.97000;0.00000;-30.99165;,
 -1.98000;-1.98000;-30.06808;,
 -1.05000;-1.05000;4.96497;,
 0.00000;-1.57500;4.96497;,
 0.00000;0.00000;4.96497;,
 1.57500;0.00000;4.96497;,
 1.05000;-1.05000;4.96497;,
 1.05000;-1.05000;-4.95385;,
 -1.98000;1.98000;-30.99164;,
 0.00000;2.97000;-30.99165;,
 0.00000;0.00000;-30.99165;,
 2.97000;0.00000;-30.06809;,
 2.97000;0.00000;-30.99166;,
 1.98000;1.98000;-30.99165;,
 2.97000;0.00000;-30.99166;,
 1.98000;-1.98000;-30.99166;,
 -1.57500;0.00000;4.96498;,
 -1.05000;1.05000;4.96499;,
 -1.05000;1.05000;-4.95383;,
 1.57500;0.00000;4.96497;,
 1.05000;1.05000;4.96497;,
 0.00000;1.57500;-4.95384;,
 0.00000;1.57500;4.96498;,
 1.05000;1.05000;4.96497;,
 0.00000;-2.97000;-30.99166;,
 -1.98000;-1.98000;-30.99165;,
 1.98000;1.98000;-30.99165;,
 1.05000;-1.05000;4.96497;,
 0.00000;1.57500;4.96498;,
 -1.05000;1.05000;4.96499;,
 0.00000;-2.97000;-30.99166;,
 1.98000;-1.98000;-30.99166;,
 1.98000;-1.98000;-30.06809;,
 -4.00905;-1.63262;-5.54113;,
 -2.17683;-2.17683;-5.54120;,
 -2.17683;-2.17683;-4.95377;,
 -2.17682;2.17683;-4.95360;,
 0.00001;3.16857;-4.95380;,
 -1.63261;4.00905;-4.95370;,
 -3.16857;0.00000;-5.54105;,
 -2.17683;-2.17683;-5.54120;,
 -4.00905;-1.63262;-5.54113;,
 0.00000;2.25000;-5.54127;,
 1.50000;1.50000;-5.54128;,
 1.80000;1.80000;-27.69795;,
 -3.16857;0.00000;-4.95362;,
 -4.00905;1.63262;-4.95361;,
 2.25000;0.00000;-5.54128;,
 1.50000;-1.50000;-5.54128;,
 1.80000;-1.80000;-27.69795;,
 0.00000;-3.16857;-5.54129;,
 2.17683;-2.17683;-5.54140;,
 1.63262;-4.00905;-5.54135;,
 -2.25000;0.00000;-5.54127;,
 -1.50000;1.50000;-5.54126;,
 -1.80000;1.80000;-27.69794;,
 2.17683;2.17683;-4.95399;,
 3.16857;0.00000;-4.95405;,
 4.00905;1.63262;-4.95402;,
 -1.05000;-1.05000;-4.95384;,
 0.00000;-1.57500;-4.95384;,
 0.00000;-1.57500;4.96497;,
 -2.17683;2.17683;-5.54104;,
 -4.00905;1.63262;-5.54104;,
 -1.57500;0.00000;-4.95384;,
 -1.05000;-1.05000;4.96497;,
 0.00000;-1.57500;-4.95384;,
 2.17683;-2.17683;-4.95397;,
 4.00905;-1.63262;-4.95401;,
 1.57500;0.00000;-4.95385;,
 1.05000;1.05000;-4.95385;,
 0.00000;3.16857;-5.54124;,
 -1.63262;4.00905;-5.54114;,
 2.17683;2.17683;-5.54142;,
 2.17683;2.17683;-4.95399;,
 4.00905;1.63262;-4.95402;,
 -1.63262;-4.00905;-5.54125;,
 0.00000;-3.16857;-5.54129;,
 0.00000;-3.16857;-4.95386;,
 -2.17683;2.17683;-5.54104;,
 -2.17682;2.17683;-4.95360;,
 -1.63261;4.00905;-4.95370;,
 1.63262;-4.00905;-5.54135;,
 2.17683;-2.17683;-5.54140;,
 2.17683;-2.17683;-4.95397;,
 1.63262;4.00905;-5.54133;,
 4.00905;-1.63262;-4.95401;,
 -4.00905;1.63262;-4.95361;,
 3.16857;0.00000;-5.54148;,
 2.17683;2.17683;-5.54142;,
 4.00905;1.63262;-5.54145;,
 0.00000;-3.16857;-4.95386;,
 -2.17683;-2.17683;-4.95377;,
 -1.63262;-4.00905;-4.95381;,
 1.63262;4.00905;-5.54133;,
 1.63262;4.00905;-4.95390;,
 -1.63262;-4.00905;-5.54125;,
 -4.00905;-1.63262;-4.95369;,
 4.00905;-1.63262;-5.54144;,
 1.63262;-4.00905;-4.95391;,
 -4.00905;-1.63262;-4.95369;,
 -3.16857;0.00000;-4.95362;,
 -3.16857;0.00000;-5.54105;,
 -4.00905;1.63262;-5.54104;,
 -1.63262;4.00905;-5.54114;,
 0.00001;3.16857;-4.95380;,
 1.63262;4.00905;-4.95390;,
 0.00000;3.16857;-5.54124;,
 4.00905;1.63262;-5.54145;,
 3.16857;0.00000;-4.95405;,
 4.00905;-1.63262;-5.54144;,
 3.16857;0.00000;-5.54148;,
 1.63262;-4.00905;-4.95391;,
 0.00000;-3.16857;-4.95386;,
 0.00000;-3.16857;-5.54129;,
 -1.63262;-4.00905;-4.95381;,
 1.98000;1.98000;-30.06808;,
 1.62000;1.62000;-30.06808;,
 2.43000;0.00000;-30.06809;,
 0.00000;2.97000;-30.99165;,
 -2.97000;0.00000;-30.06808;,
 -1.80000;1.80000;-28.26696;,
 -1.62000;1.62000;-28.26696;,
 0.00000;2.43000;-28.26697;,
 -1.80000;1.80000;-29.52204;,
 0.00000;2.70000;-29.52205;,
 0.00000;2.43000;-29.52205;,
 -1.62000;1.62000;-27.69794;,
 -2.43000;0.00000;-27.69794;,
 -2.43000;0.00000;-30.06808;,
 -1.80000;1.80000;-29.14782;,
 0.00000;2.70000;-29.14783;,
 1.80000;1.80000;-28.26697;,
 0.00000;2.70000;-28.26697;,
 1.80000;1.80000;-29.52205;,
 1.80000;1.80000;-29.14783;,
 2.70000;0.00000;-29.14784;,
 1.62000;1.62000;-27.69795;,
 0.00000;2.43000;-27.69794;,
 -1.80000;-1.80000;-29.52205;,
 0.00000;-2.70000;-29.52205;,
 0.00000;-2.70000;-29.14783;,
 -1.80000;-1.80000;-28.61063;,
 -2.70000;0.00000;-28.61063;,
 -2.43000;0.00000;-28.61063;,
 -2.70000;0.00000;-29.52204;,
 -1.80000;-1.80000;-29.14783;,
 0.00000;-2.70000;-29.52205;,
 1.80000;-1.80000;-29.52206;,
 1.80000;-1.80000;-29.14784;,
 0.00000;2.43000;-30.06808;,
 -2.70000;0.00000;-29.14783;,
 1.62000;1.62000;-28.26697;,
 2.43000;0.00000;-28.26697;,
 2.70000;0.00000;-27.69795;,
 2.43000;0.00000;-27.69795;,
 -1.62000;-1.62000;-28.61063;,
 0.00000;-2.43000;-28.61063;,
 1.62000;-1.62000;-30.06809;,
 0.00000;-2.43000;-30.06808;,
 2.70000;0.00000;-29.52206;,
 -1.80000;-1.80000;-28.26697;,
 -1.62000;-1.62000;-28.26697;,
 -2.43000;0.00000;-28.26697;,
 -1.80000;1.80000;-28.61062;,
 0.00000;-2.70000;-27.69795;,
 0.00000;-2.43000;-27.69795;,
 0.00000;-2.70000;-28.61063;,
 1.80000;-1.80000;-28.61064;,
 1.80000;-1.80000;-28.26698;,
 1.80000;1.80000;-28.61063;,
 2.70000;0.00000;-28.26697;,
 1.62000;-1.62000;-28.61064;,
 2.43000;0.00000;-28.61064;,
 0.00000;-2.70000;-28.61063;,
 0.00000;-2.70000;-28.26697;,
 -2.70000;0.00000;-28.26697;,
 2.43000;0.00000;-29.14784;,
 2.70000;0.00000;-28.61064;,
 1.62000;-1.62000;-28.26698;,
 0.00000;-2.43000;-28.26697;,
 0.00000;2.70000;-28.61063;,
 0.00000;-2.43000;-29.14783;,
 0.00000;-2.25000;-5.54128;,
 0.00000;-2.43000;-28.61063;,
 0.00000;-2.43000;-28.26697;,
 -1.80000;-1.80000;-27.69795;,
 -1.50000;-1.50000;-5.54127;,
 0.00000;-2.70000;-27.69795;,
 0.00000;-2.25000;-5.54128;,
 1.62000;-1.62000;-29.14784;,
 0.00000;-2.43000;-29.14783;,
 0.00000;-2.43000;-30.06808;,
 1.62000;-1.62000;-30.06809;,
 1.62000;-1.62000;-29.52206;,
 1.62000;1.62000;-30.06808;,
 1.62000;1.62000;-29.52205;,
 2.43000;0.00000;-29.52206;,
 -1.62000;-1.62000;-30.06808;,
 0.00000;-2.43000;-30.06808;,
 0.00000;-2.43000;-29.52205;,
 -2.43000;0.00000;-30.06808;,
 -1.62000;-1.62000;-29.52205;,
 -1.62000;1.62000;-30.06807;,
 -1.62000;1.62000;-29.52204;,
 0.00000;2.43000;-29.52205;,
 0.00000;2.43000;-30.06808;,
 -2.43000;0.00000;-29.52204;,
 2.43000;0.00000;-30.06809;,
 1.62000;1.62000;-29.14783;,
 1.62000;1.62000;-28.61063;,
 2.43000;0.00000;-28.61064;,
 -2.43000;0.00000;-29.14783;,
 -1.62000;-1.62000;-29.14783;,
 -1.62000;-1.62000;-28.61063;,
 -1.62000;1.62000;-29.14782;,
 -1.62000;1.62000;-28.61062;,
 0.00000;2.43000;-28.61063;,
 0.00000;-2.43000;-29.14783;,
 1.62000;-1.62000;-29.14784;,
 1.62000;-1.62000;-28.61064;,
 0.00000;-2.43000;-29.14783;,
 0.00000;-2.43000;-28.61063;,
 0.00000;2.43000;-29.14783;,
 -2.43000;0.00000;-28.61063;,
 2.43000;0.00000;-29.14784;,
 -1.62000;-1.62000;-28.26697;,
 0.00000;-2.43000;-28.26697;,
 0.00000;-2.43000;-27.69795;,
 0.00000;-2.43000;-28.26697;,
 1.62000;-1.62000;-28.26698;,
 1.62000;-1.62000;-27.69795;,
 -2.43000;0.00000;-28.26697;,
 -1.62000;-1.62000;-27.69795;,
 1.62000;1.62000;-28.26697;,
 1.62000;1.62000;-27.69795;,
 2.43000;0.00000;-27.69795;,
 -1.62000;1.62000;-28.26696;,
 -1.62000;1.62000;-27.69794;,
 0.00000;2.43000;-27.69794;,
 0.00000;2.43000;-28.26697;,
 -2.43000;0.00000;-27.69794;,
 2.43000;0.00000;-28.26697;,
 0.00000;2.70000;-27.69794;,
 0.00000;2.43000;-27.69794;,
 1.62000;1.62000;-29.52205;,
 0.00000;-2.43000;-30.06808;,
 -2.70000;0.00000;-27.69794;,
 -2.43000;0.00000;-27.69794;,
 2.43000;0.00000;-29.52206;,
 -1.62000;-1.62000;-30.06808;,
 0.00000;2.43000;-29.14783;,
 -1.62000;-1.62000;-27.69795;,
 0.00000;-2.43000;-27.69795;,
 -1.62000;1.62000;-29.52204;,
 -2.43000;0.00000;-29.52204;,
 -1.62000;1.62000;-30.06807;,
 1.62000;1.62000;-29.14783;,
 1.62000;-1.62000;-27.69795;,
 2.43000;0.00000;-27.69795;,
 -1.62000;-1.62000;-29.52205;,
 0.00000;-2.43000;-29.52205;,
 -2.43000;0.00000;-29.14783;,
 1.62000;-1.62000;-29.52206;,
 1.62000;1.62000;-28.61063;,
 0.00000;2.43000;-28.61063;,
 -1.62000;-1.62000;-29.14783;,
 -1.62000;1.62000;-29.14782;,
 0.00000;-2.43000;-29.52205;,
 -1.62000;1.62000;-28.61062;,
 -1.57500;0.00000;4.96498;,
 -2.97000;0.00000;-30.99165;,
 0.00000;-2.97000;-30.06808;,
 0.00000;1.57500;-4.95384;,
 -1.05000;1.05000;-4.95383;,
 -2.25000;0.00000;-5.54127;,
 -1.50000;-1.50000;-5.54127;,
 -1.57500;0.00000;-4.95384;,
 0.00000;-2.25000;-5.54128;,
 1.50000;-1.50000;-5.54128;,
 1.57500;0.00000;-4.95385;,
 1.05000;1.05000;-4.95385;,
 -1.50000;1.50000;-5.54126;,
 0.00000;-1.57500;4.96497;,
 1.05000;-1.05000;-4.95385;,
 0.00000;2.25000;-5.54127;,
 2.25000;0.00000;-5.54128;,
 1.50000;1.50000;-5.54128;,
 0.00000;-1.57500;-4.95384;,
 -1.05000;-1.05000;-4.95384;,
 0.00000;-2.70000;-29.14783;,
 1.62000;1.62000;-27.69795;,
 1.62000;-1.62000;-27.69795;,
 0.00000;-2.70000;-28.26697;,
 0.00000;-2.43000;-29.52205;,
 0.00000;-2.43000;-28.61063;,
 0.00000;-2.43000;-27.69795;,
 -1.62000;1.62000;-27.69794;,
 -1.62000;-1.62000;-27.69795;;
 
 320;
 3;0,1,2;,
 3;3,4,5;,
 3;6,0,7;,
 3;8,9,10;,
 3;11,12,13;,
 3;14,15,16;,
 3;17,18,19;,
 3;16,20,21;,
 3;22,23,24;,
 3;10,25,26;,
 3;27,28,29;,
 3;16,30,31;,
 3;16,15,32;,
 3;10,9,33;,
 3;10,34,35;,
 3;36,37,38;,
 3;39,40,41;,
 3;42,43,44;,
 3;45,46,47;,
 3;48,49,50;,
 3;51,42,52;,
 3;53,54,55;,
 3;56,57,58;,
 3;59,60,61;,
 3;62,63,64;,
 3;65,66,67;,
 3;68,45,69;,
 3;70,65,71;,
 3;72,13,12;,
 3;63,73,74;,
 3;75,76,29;,
 3;77,68,78;,
 3;24,23,28;,
 3;79,80,81;,
 3;82,83,84;,
 3;85,86,87;,
 3;88,89,90;,
 3;80,79,91;,
 3;92,90,89;,
 3;93,86,85;,
 3;94,95,96;,
 3;97,98,99;,
 3;95,77,100;,
 3;43,62,101;,
 3;46,56,102;,
 3;98,51,103;,
 3;57,94,104;,
 3;73,97,105;,
 3;106,107,108;,
 3;109,108,107;,
 3;110,87,111;,
 3;112,91,113;,
 3;114,81,115;,
 3;116,117,115;,
 3;118,119,120;,
 3;121,41,40;,
 3;122,123,124;,
 3;122,19,125;,
 3;126,4,3;,
 3;127,128,129;,
 3;130,131,132;,
 3;61,133,134;,
 3;4,126,135;,
 3;38,37,18;,
 3;130,136,137;,
 3;138,139,129;,
 3;140,141,142;,
 3;50,143,144;,
 3;145,146,147;,
 3;148,149,150;,
 3;151,145,152;,
 3;153,154,155;,
 3;141,140,131;,
 3;122,5,156;,
 3;157,136,130;,
 3;138,158,159;,
 3;50,160,161;,
 3;148,162,163;,
 3;38,164,165;,
 3;155,154,166;,
 3;167,168,169;,
 3;170,127,139;,
 3;55,171,172;,
 3;173,174,175;,
 3;176,138,177;,
 3;174,178,179;,
 3;148,180,181;,
 3;149,148,167;,
 3;127,182,169;,
 3;155,142,183;,
 3;182,127,170;,
 3;176,184,179;,
 3;175,185,186;,
 3;138,176,187;,
 3;152,147,188;,
 3;175,174,184;,
 3;55,54,189;,
 3;50,49,53;,
 3;174,173,190;,
 3;167,181,191;,
 3;61,60,48;,
 3;192,193,59;,
 3;192,194,195;,
 3;155,196,197;,
 3;198,199,200;,
 3;201,202,203;,
 3;204,205,206;,
 3;207,204,208;,
 3;209,210,211;,
 3;202,201,212;,
 3;213,210,209;,
 3;200,199,214;,
 3;215,216,217;,
 3;218,219,220;,
 3;221,222,223;,
 3;224,225,226;,
 3;219,227,228;,
 3;216,215,229;,
 3;230,222,221;,
 3;226,225,231;,
 3;232,233,234;,
 3;235,236,237;,
 3;238,232,239;,
 3;240,241,242;,
 3;243,244,245;,
 3;241,240,246;,
 3;247,244,243;,
 3;237,236,248;,
 3;61,249,250;,
 3;140,251,132;,
 3;175,177,159;,
 3;7,2,252;,
 3;192,253,254;,
 3;140,166,255;,
 3;7,256,135;,
 3;141,137,257;,
 3;192,258,259;,
 3;130,260,261;,
 3;4,262,156;,
 3;141,263,183;,
 3;55,264,265;,
 3;145,266,267;,
 3;38,17,124;,
 3;136,157,268;,
 3;154,269,255;,
 3;176,270,271;,
 3;152,272,268;,
 3;145,151,261;,
 3;170,187,271;,
 3;136,273,257;,
 3;154,153,274;,
 3;170,275,150;,
 3;0,2,7;,
 3;3,5,125;,
 3;6,7,126;,
 3;8,10,276;,
 3;11,13,75;,
 3;14,16,277;,
 3;17,19,122;,
 3;16,21,30;,
 3;22,24,70;,
 3;10,26,34;,
 3;27,29,76;,
 3;16,31,277;,
 3;16,32,20;,
 3;10,33,25;,
 3;10,35,276;,
 3;36,38,278;,
 3;39,41,106;,
 3;43,42,279;,
 3;279,42,280;,
 3;46,281,282;,
 3;281,46,45;,
 3;48,50,249;,
 3;42,283,280;,
 3;283,42,51;,
 3;53,55,160;,
 3;57,284,285;,
 3;284,57,56;,
 3;59,61,253;,
 3;63,62,286;,
 3;286,62,287;,
 3;65,67,71;,
 3;45,68,281;,
 3;281,68,288;,
 3;70,71,22;,
 3;72,12,289;,
 3;73,286,290;,
 3;286,73,63;,
 3;75,29,11;,
 3;68,291,288;,
 3;291,68,77;,
 3;24,28,27;,
 3;79,81,114;,
 3;82,84,121;,
 3;85,87,110;,
 3;88,90,118;,
 3;80,91,112;,
 3;92,89,116;,
 3;93,85,109;,
 3;95,292,293;,
 3;292,95,94;,
 3;98,294,295;,
 3;294,98,97;,
 3;77,95,291;,
 3;291,95,293;,
 3;62,279,287;,
 3;279,62,43;,
 3;56,46,284;,
 3;284,46,282;,
 3;51,98,283;,
 3;283,98,295;,
 3;94,57,292;,
 3;292,57,285;,
 3;97,73,294;,
 3;294,73,290;,
 3;106,108,39;,
 3;109,107,93;,
 3;110,111,113;,
 3;112,113,111;,
 3;114,115,117;,
 3;116,115,92;,
 3;118,120,88;,
 3;121,40,82;,
 3;122,124,17;,
 3;122,125,5;,
 3;126,3,6;,
 3;127,129,139;,
 3;130,132,260;,
 3;61,134,253;,
 3;4,135,262;,
 3;38,18,17;,
 3;130,137,131;,
 3;138,129,158;,
 3;140,142,166;,
 3;50,144,249;,
 3;145,147,152;,
 3;148,150,162;,
 3;151,152,157;,
 3;153,155,296;,
 3;141,131,137;,
 3;122,156,123;,
 3;157,130,151;,
 3;138,159,177;,
 3;50,161,297;,
 3;148,163,180;,
 3;38,165,278;,
 3;155,166,142;,
 3;167,169,182;,
 3;170,139,187;,
 3;55,172,298;,
 3;173,175,299;,
 3;176,177,184;,
 3;174,179,184;,
 3;148,181,167;,
 3;149,167,182;,
 3;127,169,128;,
 3;155,183,196;,
 3;182,170,149;,
 3;176,179,270;,
 3;175,186,299;,
 3;138,187,139;,
 3;152,188,272;,
 3;175,184,177;,
 3;55,189,171;,
 3;50,53,160;,
 3;174,190,178;,
 3;167,191,168;,
 3;61,48,249;,
 3;192,59,253;,
 3;192,195,193;,
 3;155,197,296;,
 3;198,200,300;,
 3;201,203,214;,
 3;204,206,208;,
 3;207,208,213;,
 3;209,211,212;,
 3;202,212,211;,
 3;213,209,207;,
 3;200,214,203;,
 3;215,217,231;,
 3;218,220,230;,
 3;221,223,229;,
 3;224,226,301;,
 3;219,228,220;,
 3;216,229,223;,
 3;230,221,218;,
 3;226,231,217;,
 3;232,234,239;,
 3;235,237,302;,
 3;238,239,247;,
 3;240,242,248;,
 3;243,245,246;,
 3;241,246,245;,
 3;247,243,238;,
 3;237,248,242;,
 3;61,250,303;,
 3;140,132,131;,
 3;175,159,185;,
 3;7,252,256;,
 3;192,254,304;,
 3;140,255,251;,
 3;7,135,126;,
 3;141,257,263;,
 3;192,259,194;,
 3;130,261,151;,
 3;4,156,5;,
 3;141,183,142;,
 3;55,265,160;,
 3;145,267,146;,
 3;38,124,164;,
 3;136,268,273;,
 3;154,255,166;,
 3;176,271,187;,
 3;152,268,157;,
 3;145,261,266;,
 3;170,271,275;,
 3;136,257,137;,
 3;154,274,269;,
 3;170,150,149;;
 
 MeshMaterialList {
  1;
  320;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\tex.jpg";
   }
  }
 }
 MeshNormals {
  224;
  -0.624695;-0.780869;0.000000;,
  0.000003;-0.000003;1.000000;,
  0.624695;-0.780869;-0.000000;,
  -0.000002;0.000002;-1.000000;,
  -0.707107;0.707107;0.000000;,
  -0.707107;0.707107;0.000000;,
  0.780869;0.624695;0.000000;,
  0.986394;-0.164399;-0.000000;,
  -0.986394;-0.164399;0.000000;,
  -0.986394;0.164399;0.000000;,
  0.986394;-0.164399;-0.000000;,
  -0.000002;0.000002;-1.000000;,
  -0.164399;0.986394;0.000000;,
  0.000000;0.000000;1.000000;,
  -0.000002;0.000000;-1.000000;,
  0.000004;-0.000004;1.000000;,
  0.164399;-0.986394;-0.000000;,
  -0.706977;-0.706977;0.019145;,
  0.999794;0.000000;0.020306;,
  0.000000;0.999794;0.020306;,
  0.706977;-0.706977;0.019145;,
  -0.706977;0.706977;0.019145;,
  -0.999794;-0.000000;0.020306;,
  0.706977;0.706977;0.019145;,
  0.000000;-0.999794;0.020306;,
  -0.624695;-0.780869;0.000000;,
  -0.986394;-0.164399;0.000000;,
  0.986394;0.164399;0.000000;,
  0.164399;0.986394;0.000000;,
  0.164399;-0.986394;-0.000000;,
  0.780869;-0.624695;-0.000000;,
  0.707107;0.707107;0.000000;,
  0.000058;-0.000014;1.000000;,
  -0.000085;0.000034;-1.000000;,
  0.000075;-0.000036;1.000000;,
  -0.000125;0.000035;-1.000000;,
  0.000069;0.000012;1.000000;,
  0.570128;0.821556;0.000000;,
  0.169129;-0.985594;-0.000000;,
  -0.000042;0.000006;-1.000000;,
  -0.821556;-0.570128;0.000000;,
  -0.000114;-0.000004;-1.000000;,
  -0.000080;0.000016;-1.000000;,
  -0.000125;0.000074;-1.000000;,
  -0.570128;-0.821556;0.000000;,
  0.000068;0.000004;1.000000;,
  0.000083;-0.000011;1.000000;,
  0.000087;-0.000046;1.000000;,
  -0.284728;-0.958608;0.000000;,
  0.000027;-0.000057;1.000000;,
  -0.000002;0.000005;-1.000000;,
  -0.000003;-0.000008;1.000000;,
  0.000077;0.000032;1.000000;,
  -0.000077;-0.000032;-1.000000;,
  0.000075;-0.000027;1.000000;,
  0.958608;0.284728;0.000000;,
  0.000009;-0.000023;1.000000;,
  -0.000010;0.000023;-1.000000;,
  0.000013;0.000031;1.000000;,
  -0.000012;-0.000031;-1.000000;,
  0.000041;0.000020;1.000000;,
  -0.000044;-0.000014;-1.000000;,
  0.457713;-0.889100;-0.000000;,
  0.000036;-0.000011;1.000000;,
  -0.164399;0.986394;0.000000;,
  0.986394;0.164399;0.000000;,
  0.707107;-0.707107;-0.000000;,
  -0.780869;-0.624695;0.000000;,
  -0.986394;0.164399;0.000000;,
  0.000001;-0.000003;1.000000;,
  -0.164399;-0.986394;0.000000;,
  -0.000003;0.000000;-1.000000;,
  -0.986394;-0.164399;0.000000;,
  0.986394;-0.164399;-0.000000;,
  -0.000002;-0.000001;-1.000000;,
  0.986394;0.164399;0.000000;,
  -0.164399;-0.986394;0.000000;,
  -0.986394;0.164399;0.000000;,
  0.624695;0.780869;0.000000;,
  0.707107;-0.707107;-0.000000;,
  0.780869;0.624695;0.000000;,
  0.000000;0.000003;-1.000000;,
  0.000000;0.000003;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000003;0.000000;1.000000;,
  0.000002;-0.000002;1.000000;,
  0.000000;-0.000003;1.000000;,
  0.706977;0.706977;0.019145;,
  -0.706977;0.706977;0.019145;,
  -0.706977;-0.706977;0.019145;,
  0.707107;-0.707107;-0.000000;,
  0.000003;0.000003;1.000000;,
  -0.986394;0.164399;0.000000;,
  -0.000001;-0.000002;1.000000;,
  0.624695;0.780869;0.000000;,
  0.000003;-0.000003;1.000000;,
  -0.164399;-0.986394;0.000000;,
  0.000003;0.000001;1.000000;,
  0.000002;0.000001;1.000000;,
  -0.707107;0.707107;0.000000;,
  0.986394;0.164399;0.000000;,
  -0.000003;-0.000003;-1.000000;,
  0.000002;0.000002;1.000000;,
  -0.000003;-0.000001;-1.000000;,
  0.000002;0.000001;1.000000;,
  0.000001;0.000002;-1.000000;,
  -0.986394;-0.164399;0.000000;,
  0.624695;0.780869;0.000000;,
  0.780869;0.624695;0.000000;,
  0.164399;0.986394;0.000000;,
  0.000001;-0.000002;1.000000;,
  -0.624695;-0.780869;0.000000;,
  -0.164399;0.986394;0.000000;,
  -0.000001;0.000003;-1.000000;,
  -0.000002;0.000002;-1.000000;,
  -0.000001;0.000002;-1.000000;,
  -0.000001;-0.000003;1.000000;,
  -0.000001;-0.000002;1.000000;,
  0.000003;0.000001;1.000000;,
  -0.000002;0.000001;-1.000000;,
  -0.780869;-0.624695;0.000000;,
  -0.000002;0.000002;-1.000000;,
  -0.000003;0.000002;-1.000000;,
  -0.000002;0.000003;-1.000000;,
  -0.000002;0.000002;-1.000000;,
  0.000009;-0.000006;1.000000;,
  0.000009;-0.000009;1.000000;,
  -0.000073;-0.000009;-1.000000;,
  -0.000020;0.000060;-1.000000;,
  -0.780869;-0.624695;0.000000;,
  0.821556;0.570128;0.000000;,
  0.284728;0.958608;0.000000;,
  -0.169129;-0.985594;0.000000;,
  -0.707106;0.707107;0.000011;,
  -0.707106;0.707107;0.000012;,
  -0.958608;0.284728;0.000016;,
  0.958608;-0.284728;-0.000000;,
  0.570128;-0.821556;-0.000000;,
  0.821556;-0.570128;-0.000000;,
  0.284728;-0.958608;-0.000000;,
  -0.284727;0.958609;0.000002;,
  -0.000096;-0.000032;-1.000000;,
  0.000042;-0.000004;1.000000;,
  0.000050;-0.000004;1.000000;,
  -0.000070;0.000028;-1.000000;,
  -0.000033;0.000017;-1.000000;,
  -0.889100;0.457713;0.000000;,
  -0.985594;-0.169129;0.000000;,
  -0.985594;0.169129;0.000000;,
  -0.889100;-0.457713;0.000000;,
  0.457713;0.889100;-0.000008;,
  0.457713;0.889100;-0.000008;,
  0.169129;0.985594;-0.000003;,
  -0.457714;0.889100;0.000004;,
  -0.457713;0.889100;0.000000;,
  -0.169130;0.985594;0.000000;,
  0.889100;-0.457713;-0.000000;,
  0.889100;-0.457713;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.889100;0.457713;0.000000;,
  1.000000;0.000000;0.000000;,
  -0.457713;-0.889100;0.000000;,
  -0.958608;-0.284728;0.000000;,
  0.164399;0.986394;0.000000;,
  0.000001;-0.000001;1.000000;,
  0.000000;-0.000002;1.000000;,
  -0.707107;0.707107;0.000000;,
  0.000002;0.000000;1.000000;,
  0.000000;-0.000003;1.000000;,
  0.000001;-0.000001;1.000000;,
  0.000003;0.000000;1.000000;,
  -0.000003;0.000000;-1.000000;,
  0.000000;-0.999794;0.020306;,
  0.164399;-0.986394;-0.000000;,
  -0.986394;-0.164399;0.000000;,
  -0.164399;0.986394;0.000000;,
  0.164399;0.986394;0.000000;,
  0.986394;-0.164399;-0.000000;,
  0.986394;0.164399;0.000000;,
  -0.780869;-0.624695;0.000000;,
  0.164399;-0.986394;-0.000000;,
  -0.164399;-0.986394;0.000000;,
  -0.986394;0.164399;0.000000;,
  0.986394;-0.164399;-0.000000;,
  0.624695;0.780869;0.000000;,
  0.164399;0.986394;0.000000;,
  -0.000003;0.000003;-1.000000;,
  0.000003;0.000000;1.000000;,
  -0.000002;-0.000002;-1.000000;,
  0.000002;-0.000002;1.000000;,
  0.000000;-0.000003;1.000000;,
  0.000001;-0.000001;1.000000;,
  -0.000001;0.000001;-1.000000;,
  0.000006;-0.000009;1.000000;,
  0.000000;0.000002;-1.000000;,
  -0.284728;-0.958608;0.000000;,
  0.000075;-0.000026;1.000000;,
  0.000102;-0.000102;1.000000;,
  -0.000190;0.000038;-1.000000;,
  -0.000054;-0.000050;-1.000000;,
  0.000000;0.999794;0.020306;,
  0.000112;-0.000033;1.000000;,
  -0.000048;0.000019;-1.000000;,
  -0.000089;0.000089;-1.000000;,
  -0.999794;-0.000000;0.020306;,
  0.000101;0.000004;1.000000;,
  0.000065;0.000059;1.000000;,
  -0.000163;0.000163;-1.000000;,
  0.000056;-0.000050;1.000000;,
  -0.000090;0.000036;-1.000000;,
  0.457713;-0.889100;-0.000000;,
  -0.958608;0.284728;0.000016;,
  0.958608;-0.284728;-0.000000;,
  0.958608;0.284728;0.000000;,
  0.284728;-0.958608;-0.000000;,
  -0.284728;0.958608;0.000000;,
  -0.000173;-0.000005;-1.000000;,
  -0.000106;-0.000101;-1.000000;,
  0.000039;-0.000012;1.000000;,
  0.000031;0.000031;1.000000;,
  -0.889100;0.457713;0.000000;,
  -0.889100;-0.457713;0.000000;,
  -0.457713;-0.889100;0.000000;,
  -0.958608;-0.284728;0.000000;;
  320;
  3;0,16,70;,
  3;4,4,64;,
  3;8,0,67;,
  3;1,13,15;,
  3;7,2,30;,
  3;121,14,11;,
  3;65,10,6;,
  3;11,122,3;,
  3;9,5,5;,
  3;15,13,1;,
  3;28,12,31;,
  3;11,123,124;,
  3;11,14,124;,
  3;15,13,13;,
  3;15,125,126;,
  3;16,66,66;,
  3;48,40,44;,
  3;47,46,52;,
  3;35,127,128;,
  3;19,23,87;,
  3;34,47,51;,
  3;18,20,20;,
  3;39,33,61;,
  3;22,21,88;,
  3;36,45,56;,
  3;25,29,76;,
  3;43,35,50;,
  3;26,25,129;,
  3;29,30,2;,
  3;45,32,58;,
  3;27,31,31;,
  3;42,43,53;,
  3;5,5,12;,
  3;37,130,131;,
  3;62,132,38;,
  3;133,134,135;,
  3;136,137,138;,
  3;130,37,55;,
  3;139,138,137;,
  3;140,134,133;,
  3;41,141,57;,
  3;142,143,63;,
  3;141,42,144;,
  3;46,36,54;,
  3;127,39,145;,
  3;143,34,49;,
  3;33,41,59;,
  3;32,142,60;,
  3;146,147,148;,
  3;149,148,147;,
  3;150,151,152;,
  3;153,154,155;,
  3;156,157,158;,
  3;159,160,158;,
  3;161,38,132;,
  3;162,44,40;,
  3;69,95,69;,
  3;94,6,163;,
  3;68,4,4;,
  3;85,13,118;,
  3;74,71,74;,
  3;83,83,115;,
  3;164,165,93;,
  3;66,66,10;,
  3;166,5,12;,
  3;13,84,118;,
  3;6,78,75;,
  3;119,114,119;,
  3;25,29,76;,
  3;83,82,105;,
  3;72,25,67;,
  3;29,66,66;,
  3;78,6,28;,
  3;69,167,98;,
  3;77,5,166;,
  3;13,13,117;,
  3;119,83,83;,
  3;83,83,103;,
  3;97,91,97;,
  3;66,66,73;,
  3;13,13,116;,
  3;5,5,12;,
  3;83,83,83;,
  3;29,79,79;,
  3;80,78,27;,
  3;103,101,113;,
  3;25,29,76;,
  3;26,25,129;,
  3;85,86,116;,
  3;104,168,110;,
  3;9,5,5;,
  3;113,81,113;,
  3;169,13,104;,
  3;78,80,28;,
  3;13,170,104;,
  3;79,79,7;,
  3;20,20,24;,
  3;87,23,18;,
  3;103,171,103;,
  3;13,170,104;,
  3;88,21,19;,
  3;89,17,22;,
  3;89,172,24;,
  3;104,102,104;,
  3;173,79,90;,
  3;6,94,100;,
  3;0,173,96;,
  3;174,0,67;,
  3;99,5,175;,
  3;94,6,176;,
  3;92,5,99;,
  3;90,79,177;,
  3;108,107,178;,
  3;106,111,179;,
  3;5,5,112;,
  3;180,79,79;,
  3;111,180,181;,
  3;107,108,109;,
  3;182,5,5;,
  3;79,79,183;,
  3;111,180,96;,
  3;180,79,79;,
  3;72,111,120;,
  3;108,184,100;,
  3;5,4,12;,
  3;184,108,185;,
  3;77,4,5;,
  3;79,79,183;,
  3;83,71,119;,
  3;113,186,74;,
  3;169,168,117;,
  3;13,187,97;,
  3;115,82,115;,
  3;113,81,113;,
  3;13,13,93;,
  3;110,170,104;,
  3;115,114,83;,
  3;74,188,115;,
  3;164,13,98;,
  3;110,189,110;,
  3;83,83,83;,
  3;115,114,103;,
  3;97,190,69;,
  3;191,168,117;,
  3;103,101,113;,
  3;113,186,74;,
  3;13,13,117;,
  3;115,82,115;,
  3;192,71,74;,
  3;191,13,104;,
  3;103,171,103;,
  3;192,83,105;,
  3;0,70,67;,
  3;4,64,163;,
  3;8,67,68;,
  3;1,15,193;,
  3;7,30,27;,
  3;121,11,194;,
  3;65,6,94;,
  3;11,3,123;,
  3;9,5,26;,
  3;15,1,125;,
  3;28,31,31;,
  3;11,124,194;,
  3;11,124,122;,
  3;15,13,13;,
  3;15,126,193;,
  3;16,66,70;,
  3;48,44,195;,
  3;46,47,196;,
  3;196,47,197;,
  3;127,198,199;,
  3;198,127,35;,
  3;19,87,200;,
  3;47,201,197;,
  3;201,47,34;,
  3;18,20,18;,
  3;33,202,203;,
  3;202,33,39;,
  3;22,88,204;,
  3;45,36,205;,
  3;205,36,206;,
  3;25,76,129;,
  3;35,43,198;,
  3;198,43,207;,
  3;26,129,9;,
  3;29,2,76;,
  3;32,205,208;,
  3;205,32,45;,
  3;27,31,7;,
  3;43,209,207;,
  3;209,43,42;,
  3;5,12,28;,
  3;37,131,131;,
  3;62,38,210;,
  3;133,135,211;,
  3;136,138,212;,
  3;130,55,213;,
  3;139,137,214;,
  3;140,133,215;,
  3;141,216,217;,
  3;216,141,41;,
  3;143,218,219;,
  3;218,143,142;,
  3;42,141,209;,
  3;209,141,217;,
  3;36,196,206;,
  3;196,36,46;,
  3;39,127,202;,
  3;202,127,199;,
  3;34,143,201;,
  3;201,143,219;,
  3;41,33,216;,
  3;216,33,203;,
  3;142,32,218;,
  3;218,32,208;,
  3;146,148,220;,
  3;149,147,221;,
  3;150,152,155;,
  3;153,155,152;,
  3;156,158,160;,
  3;159,158,159;,
  3;161,132,222;,
  3;162,40,223;,
  3;69,69,190;,
  3;94,163,64;,
  3;68,4,8;,
  3;85,118,84;,
  3;74,74,188;,
  3;83,115,82;,
  3;164,93,13;,
  3;66,10,65;,
  3;166,12,28;,
  3;13,118,13;,
  3;6,75,73;,
  3;119,119,71;,
  3;25,76,67;,
  3;83,105,83;,
  3;72,67,77;,
  3;29,66,76;,
  3;78,28,12;,
  3;69,98,95;,
  3;77,166,72;,
  3;13,117,168;,
  3;119,83,114;,
  3;83,103,171;,
  3;97,97,187;,
  3;66,73,75;,
  3;13,116,86;,
  3;5,12,28;,
  3;83,83,83;,
  3;29,79,76;,
  3;80,27,7;,
  3;103,113,81;,
  3;25,76,129;,
  3;26,129,9;,
  3;85,116,13;,
  3;104,110,102;,
  3;9,5,26;,
  3;113,113,186;,
  3;169,104,170;,
  3;78,28,12;,
  3;13,104,13;,
  3;79,7,27;,
  3;20,24,172;,
  3;87,18,18;,
  3;103,103,101;,
  3;13,104,13;,
  3;88,19,200;,
  3;89,22,204;,
  3;89,24,17;,
  3;104,104,170;,
  3;173,90,96;,
  3;6,100,177;,
  3;0,96,67;,
  3;174,67,92;,
  3;99,175,176;,
  3;94,176,175;,
  3;92,99,174;,
  3;90,177,100;,
  3;108,178,183;,
  3;106,179,182;,
  3;5,112,109;,
  3;180,79,181;,
  3;111,181,179;,
  3;107,109,112;,
  3;182,5,106;,
  3;79,183,178;,
  3;111,96,120;,
  3;180,79,96;,
  3;72,120,77;,
  3;108,100,183;,
  3;5,12,185;,
  3;184,185,12;,
  3;77,5,72;,
  3;79,183,100;,
  3;83,119,83;,
  3;113,74,71;,
  3;169,117,13;,
  3;13,97,13;,
  3;115,115,114;,
  3;113,113,186;,
  3;13,93,165;,
  3;110,104,189;,
  3;115,83,83;,
  3;74,115,82;,
  3;164,98,167;,
  3;110,110,168;,
  3;83,83,83;,
  3;115,103,171;,
  3;97,69,91;,
  3;191,117,13;,
  3;103,113,81;,
  3;113,74,71;,
  3;13,117,168;,
  3;115,115,114;,
  3;192,74,83;,
  3;191,104,170;,
  3;103,103,101;,
  3;192,105,82;;
 }
 MeshTextureCoords {
  305;
  0.419960;0.151560;,
  0.444730;0.222480;,
  0.415140;0.232810;,
  0.304580;0.042290;,
  0.291510;0.077400;,
  0.223490;0.061590;,
  0.376540;0.086700;,
  0.392520;0.164820;,
  0.160470;0.772860;,
  0.125280;0.789980;,
  0.125850;0.737480;,
  0.259260;0.772410;,
  0.259260;0.736880;,
  0.559450;0.736880;,
  0.190370;0.965350;,
  0.124000;0.997600;,
  0.125130;0.898600;,
  0.104450;0.109290;,
  0.085180;0.089630;,
  0.145550;0.049730;,
  0.026120;0.897470;,
  0.059880;0.831840;,
  0.259260;0.914530;,
  0.259260;0.879000;,
  0.559450;0.879000;,
  0.073350;0.736910;,
  0.091230;0.702100;,
  0.559450;0.843470;,
  0.259260;0.843470;,
  0.259260;0.807940;,
  0.126250;0.799590;,
  0.191880;0.833350;,
  0.058370;0.963850;,
  0.090470;0.772100;,
  0.126420;0.684980;,
  0.161230;0.702860;,
  0.020830;0.223510;,
  0.044160;0.149150;,
  0.072120;0.163120;,
  0.811190;0.653300;,
  0.874930;0.653300;,
  0.874930;0.672890;,
  0.867340;0.135440;,
  0.720780;0.068670;,
  0.830700;0.012080;,
  0.866420;0.428880;,
  0.929050;0.279870;,
  0.991110;0.392010;,
  0.577550;0.843060;,
  0.577270;0.812270;,
  0.984080;0.806110;,
  0.928560;0.280330;,
  0.990700;0.172080;,
  0.576620;0.781490;,
  0.575410;0.750720;,
  0.981850;0.732250;,
  0.867440;0.135330;,
  0.721190;0.069180;,
  0.830710;0.012310;,
  0.576620;0.904630;,
  0.577270;0.873840;,
  0.984080;0.880000;,
  0.574230;0.135440;,
  0.514120;0.280330;,
  0.450870;0.172080;,
  0.559450;0.950050;,
  0.559450;0.985580;,
  0.259260;0.985580;,
  0.721320;0.494580;,
  0.830870;0.551380;,
  0.559450;0.914530;,
  0.259260;0.950050;,
  0.559450;0.701350;,
  0.574230;0.428550;,
  0.450870;0.391910;,
  0.559450;0.772410;,
  0.559450;0.807940;,
  0.574510;0.427860;,
  0.611800;0.551450;,
  0.375000;0.653280;,
  0.375000;0.672870;,
  0.311250;0.672870;,
  0.938670;0.653300;,
  0.999910;0.653300;,
  0.999910;0.672890;,
  0.624960;0.653300;,
  0.624960;0.672890;,
  0.561220;0.672880;,
  0.061290;0.653270;,
  0.125030;0.653270;,
  0.125030;0.672870;,
  0.438740;0.653280;,
  0.188770;0.672870;,
  0.688710;0.672890;,
  0.574980;0.135430;,
  0.513460;0.280000;,
  0.451950;0.171740;,
  0.720780;0.495320;,
  0.867340;0.428550;,
  0.830700;0.551910;,
  0.450910;0.391620;,
  0.610870;0.012080;,
  0.991050;0.172690;,
  0.990700;0.391910;,
  0.611080;0.011820;,
  0.610870;0.551910;,
  0.811190;0.672890;,
  0.749950;0.672890;,
  0.749950;0.653300;,
  0.688710;0.653300;,
  0.561220;0.653290;,
  0.499980;0.672880;,
  0.438740;0.672880;,
  0.499980;0.653290;,
  0.311250;0.653280;,
  0.250010;0.672870;,
  0.188770;0.653270;,
  0.250010;0.653270;,
  0.061290;0.672860;,
  0.000050;0.672860;,
  0.000050;0.653270;,
  0.938670;0.672890;,
  0.158770;0.078100;,
  0.164250;0.092010;,
  0.116080;0.121140;,
  0.221070;0.028810;,
  0.353400;0.109850;,
  0.376410;0.571150;,
  0.376390;0.580280;,
  0.304130;0.580350;,
  0.376710;0.602150;,
  0.304140;0.602200;,
  0.304240;0.591780;,
  0.988490;0.881570;,
  0.987780;0.914820;,
  0.341460;0.123550;,
  0.376660;0.617970;,
  0.304480;0.616620;,
  0.231700;0.569960;,
  0.304220;0.569930;,
  0.231960;0.600840;,
  0.231910;0.616660;,
  0.159340;0.616180;,
  0.988530;0.807550;,
  0.988830;0.840800;,
  0.521740;0.601950;,
  0.603620;0.600570;,
  0.603740;0.618170;,
  0.521400;0.556430;,
  0.448980;0.557050;,
  0.448950;0.546620;,
  0.449280;0.602640;,
  0.521460;0.617190;,
  0.004890;0.600650;,
  0.087170;0.601620;,
  0.086890;0.616870;,
  0.223580;0.079690;,
  0.448850;0.617030;,
  0.231670;0.579130;,
  0.159200;0.579960;,
  0.983300;0.769170;,
  0.987780;0.771290;,
  0.521270;0.547040;,
  0.597320;0.544180;,
  0.086040;0.168800;,
  0.073330;0.228330;,
  0.159780;0.601780;,
  0.521170;0.570440;,
  0.521080;0.579860;,
  0.448650;0.580730;,
  0.376450;0.556620;,
  0.979950;0.695350;,
  0.984490;0.697340;,
  0.004890;0.555280;,
  0.086980;0.556140;,
  0.086750;0.570150;,
  0.231730;0.555430;,
  0.159170;0.569530;,
  0.087060;0.546720;,
  0.159500;0.545850;,
  0.603160;0.555210;,
  0.603260;0.571300;,
  0.448610;0.570310;,
  0.159380;0.626610;,
  0.159540;0.556270;,
  0.086880;0.579540;,
  0.010830;0.582410;,
  0.303930;0.556650;,
  0.597890;0.629290;,
  0.573830;0.719970;,
  0.010710;0.544170;,
  0.597440;0.582410;,
  0.981850;0.953860;,
  0.575410;0.935390;,
  0.979950;0.990760;,
  0.573830;0.966140;,
  0.087040;0.626270;,
  0.010890;0.629290;,
  0.033270;0.475340;,
  0.093680;0.475340;,
  0.093680;0.493550;,
  0.214480;0.475340;,
  0.214480;0.493550;,
  0.154080;0.493550;,
  0.456100;0.475340;,
  0.516510;0.475340;,
  0.516510;0.493550;,
  0.395700;0.475340;,
  0.456100;0.493550;,
  0.335290;0.475340;,
  0.335290;0.493550;,
  0.274890;0.493550;,
  0.274890;0.475340;,
  0.395700;0.493550;,
  0.154080;0.475340;,
  0.214480;0.448010;,
  0.214480;0.465930;,
  0.154080;0.465930;,
  0.395700;0.448010;,
  0.456100;0.448010;,
  0.456100;0.465930;,
  0.335290;0.448010;,
  0.335290;0.465930;,
  0.274890;0.465930;,
  0.033270;0.448010;,
  0.093680;0.448010;,
  0.093680;0.465930;,
  0.516510;0.448010;,
  0.516510;0.465930;,
  0.274890;0.448010;,
  0.395700;0.465930;,
  0.154080;0.448010;,
  0.456100;0.501690;,
  0.516510;0.501690;,
  0.516510;0.520670;,
  0.033270;0.501690;,
  0.093680;0.501690;,
  0.093680;0.520660;,
  0.395700;0.501690;,
  0.456100;0.520670;,
  0.214480;0.501690;,
  0.214480;0.520660;,
  0.154080;0.520660;,
  0.335290;0.501690;,
  0.335290;0.520670;,
  0.274890;0.520660;,
  0.274890;0.501690;,
  0.395700;0.520670;,
  0.154080;0.501690;,
  0.984420;0.843060;,
  0.988830;0.845310;,
  0.231990;0.591720;,
  0.395800;0.229890;,
  0.983300;0.916940;,
  0.987640;0.919330;,
  0.159740;0.591360;,
  0.378560;0.171940;,
  0.304380;0.627040;,
  0.986200;0.955560;,
  0.984490;0.988770;,
  0.376740;0.592980;,
  0.449250;0.592210;,
  0.285870;0.092570;,
  0.231890;0.625830;,
  0.986340;0.733550;,
  0.987640;0.766780;,
  0.521590;0.592550;,
  0.597740;0.589530;,
  0.448890;0.627460;,
  0.087260;0.592190;,
  0.231760;0.546300;,
  0.304020;0.546230;,
  0.521370;0.626620;,
  0.376640;0.627090;,
  0.010740;0.589530;,
  0.376480;0.547460;,
  0.178350;0.738050;,
  0.224130;0.899720;,
  0.054060;0.232810;,
  0.720780;0.175960;,
  0.791480;0.211300;,
  0.790350;0.353620;,
  0.826670;0.282110;,
  0.826820;0.282000;,
  0.791950;0.211180;,
  0.721210;0.175910;,
  0.614750;0.282000;,
  0.650090;0.211300;,
  0.721300;0.387850;,
  0.259260;0.701350;,
  0.650090;0.352690;,
  0.649440;0.352570;,
  0.649940;0.211250;,
  0.614730;0.281640;,
  0.720780;0.388030;,
  0.791480;0.352690;,
  0.005010;0.618250;,
  0.988490;0.804540;,
  0.986200;0.730550;,
  0.004990;0.571370;,
  0.033270;0.493550;,
  0.033270;0.465930;,
  0.033270;0.520660;,
  0.988530;0.878560;,
  0.986340;0.952560;;
 }
}
