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
 44;
 4.22110;-9.48878;0.69587;,
 0.53860;-0.22628;-1.20614;,
 -0.33080;-0.22628;-1.20614;,
 3.35170;-9.48878;0.69587;,
 4.22110;-9.48878;-0.02233;,
 0.53860;-0.22628;-1.92434;,
 3.02887;-12.03174;2.65452;,
 3.39110;-8.80343;1.84095;,
 1.99591;-9.91664;0.73235;,
 0.95594;-12.03174;0.87860;,
 5.53230;-12.03174;2.91613;,
 5.14851;-9.91664;2.13597;,
 3.72554;-10.51778;-3.15247;,
 3.45187;-12.03175;-4.72734;,
 8.01011;-12.03174;-2.64914;,
 6.87814;-10.51778;-1.74884;,
 6.63865;-12.03175;-5.45317;,
 8.01011;-12.03174;-2.64914;,
 5.53230;-12.03174;2.91613;,
 3.02887;-12.03174;2.65452;,
 3.45187;-12.03175;-4.72734;,
 0.95594;-12.03174;0.87860;,
 6.15869;-10.80721;-4.37518;,
 6.63865;-12.03175;-5.45317;,
 5.00572;-9.40457;-1.74392;,
 1.54120;-9.53277;0.71750;,
 3.31624;-8.23539;2.00244;,
 2.83094;-12.40039;3.07794;,
 0.37516;-12.12277;0.97210;,
 5.47073;-9.55560;2.48263;,
 5.83744;-12.16960;3.40830;,
 3.05020;-11.99830;-5.16513;,
 3.47571;-10.05909;-3.43803;,
 7.27431;-10.07348;-1.74748;,
 8.60447;-12.05300;-2.62776;,
 5.83744;-12.16960;3.40830;,
 8.60447;-12.05300;-2.62776;,
 6.86395;-12.35354;-5.90037;,
 2.83094;-12.40039;3.07794;,
 3.05020;-11.99830;-5.16513;,
 0.37516;-12.12277;0.97210;,
 6.86395;-12.35354;-5.90037;,
 6.30405;-10.31444;-4.67560;,
 5.08100;-8.83714;-1.90738;;
 
 31;
 4;0,1,2,3;,
 4;4,5,1,0;,
 4;3,2,5,4;,
 4;6,7,8,9;,
 4;10,11,7,6;,
 4;8,12,13,9;,
 4;14,15,11,10;,
 4;16,17,18,19;,
 4;20,16,19,21;,
 4;12,22,23,13;,
 4;22,15,14,23;,
 3;7,11,15;,
 3;8,7,12;,
 3;12,7,24;,
 3;24,7,15;,
 3;24,15,22;,
 3;12,24,22;,
 4;25,26,27,28;,
 4;26,29,30,27;,
 4;31,32,25,28;,
 4;29,33,34,30;,
 4;35,36,37,38;,
 4;38,37,39,40;,
 4;41,42,32,31;,
 4;34,33,42,41;,
 3;29,26,33;,
 3;26,25,32;,
 3;26,32,43;,
 3;26,43,33;,
 3;33,43,42;,
 3;43,32,42;;
 
 MeshMaterialList {
  1;
  31;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "C:\\Users\\game114\\Desktop\\2年生後期_3Dスクロールアクション\\game_project\\14 ギミック\\DATA\\TEXTURE\\MODEL\\RoboBasic.png";
   }
  }
 }
 MeshNormals {
  43;
  0.929253;0.369444;-0.000000;,
  0.126788;0.952803;-0.275847;,
  0.046027;0.847945;-0.528082;,
  0.000000;-1.000000;0.000000;,
  0.847628;0.529811;0.028771;,
  0.660371;0.750935;0.002387;,
  -0.420633;0.771674;-0.477062;,
  -0.544939;0.594979;-0.590796;,
  0.000000;0.201147;0.979561;,
  -0.052916;0.287486;0.956322;,
  -0.380749;0.310661;0.870931;,
  -0.664680;0.298015;0.685119;,
  -0.880392;-0.474247;-0.000757;,
  -0.125118;-0.954866;0.269400;,
  -0.056860;-0.826691;0.559775;,
  -0.000372;0.999929;0.011945;,
  -0.668723;-0.743469;0.007934;,
  0.416507;-0.764691;0.491700;,
  0.547891;-0.540179;0.638766;,
  0.092980;-0.285864;-0.953749;,
  0.637921;-0.297704;-0.710232;,
  0.382907;-0.305721;-0.871732;,
  -0.588962;-0.380555;-0.712953;,
  -0.641373;0.672595;-0.369131;,
  -0.807979;0.442649;-0.388884;,
  0.733721;0.633665;0.245199;,
  0.864611;0.348026;0.362388;,
  0.000001;-1.000000;0.000000;,
  -0.000000;-1.000000;0.000000;,
  -0.209334;0.668080;-0.714036;,
  0.708048;0.634966;-0.309007;,
  0.514844;0.852211;0.093127;,
  -0.408593;0.854506;-0.320736;,
  0.650235;-0.667787;0.362291;,
  0.825349;-0.409643;0.388576;,
  -0.733037;-0.629626;-0.257348;,
  -0.870714;-0.320539;-0.372978;,
  -0.086641;0.995873;-0.027038;,
  0.085901;0.995007;0.050820;,
  0.184624;-0.586113;0.788914;,
  -0.747210;-0.550999;0.371587;,
  -0.227971;-0.972787;0.041412;,
  0.411401;-0.860492;0.300504;;
  31;
  4;8,8,8,8;,
  4;0,0,0,0;,
  4;22,22,22,22;,
  4;10,10,11,11;,
  4;9,9,10,10;,
  4;23,6,7,24;,
  4;4,5,25,26;,
  4;3,27,27,3;,
  4;28,3,3,28;,
  4;6,2,29,7;,
  4;30,5,4,30;,
  3;31,25,5;,
  3;23,32,6;,
  3;6,32,1;,
  3;1,31,5;,
  3;1,5,2;,
  3;6,1,2;,
  4;20,21,21,20;,
  4;21,19,19,21;,
  4;18,17,33,34;,
  4;35,16,12,36;,
  4;37,37,15,15;,
  4;15,15,38,38;,
  4;39,14,17,18;,
  4;12,16,40,40;,
  3;35,41,16;,
  3;42,33,17;,
  3;41,17,13;,
  3;41,13,16;,
  3;16,13,14;,
  3;13,17,14;;
 }
 MeshTextureCoords {
  44;
  0.918591;0.477719;,
  0.921173;0.472586;,
  0.921173;0.472586;,
  0.918591;0.477719;,
  0.920063;0.477719;,
  0.922645;0.472586;,
  0.887852;0.988219;,
  0.887864;0.935944;,
  0.936864;0.925403;,
  0.987065;0.963051;,
  0.797084;0.963051;,
  0.838864;0.925403;,
  0.936864;0.824732;,
  0.987065;0.764851;,
  0.797084;0.766290;,
  0.838864;0.824732;,
  0.690352;0.711571;,
  0.612084;0.766290;,
  0.612084;0.963051;,
  0.690352;0.988219;,
  0.789565;0.764851;,
  0.789565;0.963051;,
  0.887864;0.774860;,
  0.887852;0.711571;,
  0.887383;0.842868;,
  0.881904;0.474107;,
  0.882284;0.474305;,
  0.882181;0.474465;,
  0.881647;0.474144;,
  0.882749;0.474373;,
  0.882833;0.474515;,
  0.882242;0.473190;,
  0.882330;0.473458;,
  0.883153;0.473715;,
  0.883443;0.473575;,
  0.893675;0.476315;,
  0.894286;0.475375;,
  0.893913;0.474873;,
  0.893024;0.476265;,
  0.893085;0.474990;,
  0.892490;0.475944;,
  0.883070;0.473073;,
  0.882947;0.473262;,
  0.882675;0.473696;;
 }
}
