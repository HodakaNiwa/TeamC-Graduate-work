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
 104;
 12.14271;-3.43446;3.15345;,
 11.21297;-0.01112;2.94930;,
 9.13591;-0.01112;5.11873;,
 -9.27004;-3.43446;5.70789;,
 -5.00535;-3.43446;7.78275;,
 -4.61376;-0.01112;6.85247;,
 -12.19321;-3.43446;-0.02089;,
 -12.14271;-3.43446;3.15345;,
 -11.21297;-0.01112;2.94930;,
 -7.56863;-3.43446;-5.61027;,
 -7.70289;-0.01112;-5.15460;,
 -4.00959;-0.01112;-6.33281;,
 -11.21297;-0.01112;2.94930;,
 -9.13591;-0.01112;5.11873;,
 -5.87670;-0.01112;-0.02089;,
 4.01800;-3.43446;-6.80556;,
 0.00000;-3.43446;-6.97080;,
 0.00000;-0.01112;-6.61433;,
 4.61376;-0.01112;6.85247;,
 5.87670;-0.01112;-0.02089;,
 0.00000;-0.01112;-0.02089;,
 5.00535;-3.43446;7.78275;,
 4.61376;-0.01112;6.85247;,
 0.00000;-0.01112;7.62529;,
 12.19321;-3.43446;-0.02089;,
 10.00086;-3.43446;-4.07631;,
 9.75734;-0.01112;-3.75045;,
 4.00959;-0.01112;-6.33281;,
 0.00000;-0.01112;-6.61433;,
 -7.70289;-0.01112;-5.15460;,
 -9.75734;-0.01112;-3.75045;,
 -10.00086;-3.43446;-4.07631;,
 -9.75734;-0.01112;-3.75045;,
 0.00000;-11.36532;6.84435;,
 0.00000;-11.36532;2.33781;,
 4.05502;-10.82310;7.37256;,
 -4.61376;-0.01112;6.85247;,
 0.00000;-3.43446;8.12991;,
 -4.00959;-0.01112;-6.33281;,
 6.01099;-10.45868;2.31006;,
 7.74347;-10.38777;5.78267;,
 -4.01800;-3.43446;-6.80556;,
 9.13591;-0.01112;5.11873;,
 9.75734;-0.01112;-3.75045;,
 11.80461;-0.01112;-0.02089;,
 9.27004;-3.43446;5.70789;,
 7.56863;-3.43446;-5.61027;,
 4.00959;-0.01112;-6.33281;,
 -7.74347;-10.38777;5.78267;,
 -4.05502;-10.82310;7.37256;,
 -3.03680;-10.82310;-5.13137;,
 -6.28178;-10.38777;-4.47728;,
 0.00000;-11.36532;6.84435;,
 3.03680;-10.82310;-5.13137;,
 0.00000;-11.36532;-5.56124;,
 4.05502;-10.82310;7.37256;,
 -10.59563;-10.43108;3.07831;,
 7.74347;-10.38777;5.78267;,
 9.05425;-10.43108;-3.34223;,
 6.28178;-10.38777;-4.47728;,
 12.19321;-3.43446;-0.02089;,
 11.80461;-0.01112;-0.02089;,
 7.70289;-0.01112;-5.15460;,
 -12.19321;-3.43446;-0.02089;,
 -11.80461;-0.01112;-0.02089;,
 -9.13591;-0.01112;5.11873;,
 -4.05502;-10.82310;7.37256;,
 -7.74347;-10.38777;5.78267;,
 -6.01099;-10.45868;2.31006;,
 6.01099;-9.95207;-1.29693;,
 6.01099;-9.72591;-0.10029;,
 0.00000;-11.36532;-0.14570;,
 -6.01099;-9.95207;-1.29693;,
 -6.01099;-9.72591;-0.10029;,
 -11.03514;-10.41848;-0.02693;,
 11.03514;-10.41848;-0.02693;,
 9.05425;-10.43108;-3.34223;,
 6.01099;-10.45868;-2.34699;,
 0.00000;-11.36532;-1.37065;,
 -6.01099;-10.45868;-2.34699;,
 -9.05425;-10.43108;-3.34223;,
 -3.03680;-10.82310;-5.13137;,
 0.00000;-11.36532;-2.36082;,
 3.03680;-10.82310;-5.13137;,
 -6.28178;-10.38777;-4.47728;,
 6.28178;-10.38777;-4.47728;,
 6.01099;-9.95207;1.15266;,
 0.00000;-11.36532;1.17007;,
 -6.01099;-9.95207;1.15266;,
 -10.59563;-10.43108;3.07831;,
 10.59563;-10.43108;3.07831;,
 -9.05425;-10.43108;-3.34223;,
 -11.03514;-10.41848;-0.02693;,
 10.59563;-10.43108;3.07831;,
 11.03514;-10.41848;-0.02693;,
 11.03514;-10.41848;-0.02693;,
 0.00000;-11.36532;-5.56124;,
 -11.80461;-0.01112;-0.02089;,
 -11.80461;-0.01112;-0.02089;,
 0.00000;-0.01112;7.62529;,
 11.80461;-0.01112;-0.02089;,
 11.21297;-0.01112;2.94930;,
 7.70289;-0.01112;-5.15460;,
 -11.03514;-10.41848;-0.02693;;
 
 128;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;9,10,11;,
 3;12,13,14;,
 3;15,16,17;,
 3;18,19,20;,
 3;21,22,23;,
 3;24,25,26;,
 3;27,28,20;,
 3;14,29,30;,
 3;31,32,10;,
 3;33,34,35;,
 3;20,14,36;,
 3;4,37,23;,
 3;20,28,38;,
 3;35,39,40;,
 3;41,11,17;,
 3;42,19,18;,
 3;43,19,44;,
 3;45,2,22;,
 3;46,15,47;,
 3;48,49,4;,
 3;50,51,9;,
 3;49,52,37;,
 3;50,41,16;,
 3;53,15,46;,
 3;53,54,16;,
 3;55,21,37;,
 3;56,48,3;,
 3;57,45,21;,
 3;25,58,59;,
 3;60,61,1;,
 3;25,46,62;,
 3;63,64,32;,
 3;7,3,65;,
 3;66,67,68;,
 3;33,66,34;,
 3;69,70,71;,
 3;72,73,74;,
 3;75,70,69;,
 3;71,73,72;,
 3;76,69,77;,
 3;78,72,79;,
 3;77,69,78;,
 3;79,72,80;,
 3;79,81,82;,
 3;77,82,83;,
 3;79,84,81;,
 3;85,77,83;,
 3;86,87,71;,
 3;88,89,74;,
 3;75,90,86;,
 3;71,87,88;,
 3;39,34,87;,
 3;68,67,89;,
 3;90,40,39;,
 3;87,34,68;,
 3;56,7,6;,
 3;91,92,63;,
 3;51,91,31;,
 3;93,94,60;,
 3;24,95,58;,
 3;93,0,45;,
 3;82,81,96;,
 3;83,82,96;,
 3;35,34,39;,
 3;34,66,68;,
 3;0,2,45;,
 3;3,5,65;,
 3;6,8,97;,
 3;9,11,41;,
 3;98,12,14;,
 3;14,13,36;,
 3;15,17,47;,
 3;18,20,99;,
 3;21,23,37;,
 3;24,26,100;,
 3;27,20,19;,
 3;29,14,38;,
 3;14,30,98;,
 3;31,10,9;,
 3;20,36,99;,
 3;4,23,5;,
 3;20,38,14;,
 3;41,17,16;,
 3;19,101,44;,
 3;101,19,42;,
 3;27,19,102;,
 3;102,19,43;,
 3;45,22,21;,
 3;46,47,62;,
 3;48,4,3;,
 3;50,9,41;,
 3;49,37,4;,
 3;50,16,54;,
 3;53,46,59;,
 3;53,16,15;,
 3;55,37,52;,
 3;56,3,7;,
 3;57,21,55;,
 3;25,59,46;,
 3;60,1,0;,
 3;25,62,26;,
 3;63,32,31;,
 3;7,65,8;,
 3;69,71,78;,
 3;72,74,80;,
 3;75,69,76;,
 3;71,72,78;,
 3;76,77,85;,
 3;78,79,82;,
 3;77,78,82;,
 3;79,80,84;,
 3;86,71,70;,
 3;88,74,73;,
 3;75,86,70;,
 3;71,88,73;,
 3;39,87,86;,
 3;68,89,88;,
 3;90,39,86;,
 3;87,68,88;,
 3;56,6,103;,
 3;91,63,31;,
 3;51,31,9;,
 3;93,60,0;,
 3;24,58,25;,
 3;93,45,57;;
 
 MeshMaterialList {
  1;
  128;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  79;
  0.468385;0.108675;-0.876815;,
  0.581307;0.188754;0.791488;,
  0.000000;1.000000;0.000000;,
  -0.468385;0.108675;-0.876815;,
  0.373936;-0.192881;-0.907176;,
  0.053385;-0.998554;-0.006244;,
  -0.512582;-0.111310;0.851393;,
  -0.373936;-0.192881;-0.907176;,
  0.000000;-0.174989;-0.984570;,
  0.000000;0.212728;0.977111;,
  0.000000;0.120271;-0.992741;,
  0.000000;-1.000000;-0.000000;,
  -0.982168;0.111491;-0.151381;,
  -0.119774;-0.992688;0.014983;,
  0.119774;-0.992688;0.014983;,
  -0.125161;-0.111247;0.985880;,
  -0.230759;0.127962;-0.964560;,
  -0.312893;0.245416;0.917534;,
  -0.185050;-0.212058;-0.959577;,
  0.128490;-0.991597;-0.015020;,
  0.185050;-0.212058;-0.959577;,
  -0.374662;-0.057911;-0.925351;,
  -0.266960;0.077399;0.960594;,
  0.548544;0.038828;0.835219;,
  0.266960;0.077399;0.960594;,
  -0.000000;-0.099876;-0.995000;,
  -0.548544;0.038828;0.835219;,
  -0.978011;-0.014019;-0.208080;,
  0.159063;-0.021375;-0.987037;,
  -0.159063;-0.021375;-0.987037;,
  0.000000;-0.026225;0.999656;,
  0.374662;-0.057911;-0.925351;,
  0.978011;-0.014019;-0.208080;,
  -0.883327;-0.180654;0.432548;,
  -0.012730;-0.987127;0.159432;,
  0.731894;-0.158837;-0.662647;,
  -0.007218;-0.990713;-0.135776;,
  -0.650556;-0.039625;-0.758424;,
  -0.874165;0.061318;0.481743;,
  0.874165;0.061318;0.481743;,
  0.650556;-0.039625;-0.758424;,
  0.926929;0.212089;0.309549;,
  0.788297;0.110746;-0.605247;,
  -0.926929;0.212089;0.309549;,
  -0.064634;-0.997901;0.004102;,
  0.064634;-0.997901;0.004102;,
  0.000000;-0.999998;0.002064;,
  0.000000;-0.988992;0.147969;,
  0.101251;-0.970070;0.220710;,
  -0.101251;-0.970070;0.220710;,
  0.000000;-0.999949;-0.010114;,
  0.134000;-0.982045;0.132787;,
  -0.134000;-0.982045;0.132787;,
  -0.092870;-0.975794;-0.197992;,
  0.092870;-0.975794;-0.197992;,
  0.000000;-0.990721;-0.135915;,
  -0.116827;-0.983355;-0.139155;,
  0.116827;-0.983355;-0.139155;,
  0.000000;-0.999990;-0.004574;,
  0.312893;0.245416;0.917534;,
  -0.788297;0.110746;-0.605247;,
  0.230759;0.127962;-0.964560;,
  0.000000;-0.160008;0.987116;,
  0.125161;-0.111247;0.985880;,
  0.512582;-0.111310;0.851393;,
  0.982168;0.111491;-0.151381;,
  -0.581307;0.188754;0.791488;,
  -0.128490;-0.991597;-0.015020;,
  -0.053385;-0.998554;-0.006244;,
  0.012730;-0.987127;0.159432;,
  -0.153688;-0.988002;-0.015214;,
  0.153688;-0.988002;-0.015214;,
  -0.067242;-0.997432;-0.024654;,
  0.067242;-0.997432;-0.024654;,
  0.007218;-0.990713;-0.135776;,
  -0.731894;-0.158837;-0.662647;,
  -0.967464;-0.160253;-0.195788;,
  0.883327;-0.180654;0.432548;,
  0.967464;-0.160253;-0.195788;;
  128;
  3;39,41,1;,
  3;26,22,17;,
  3;27,38,43;,
  3;21,3,16;,
  3;2,2,2;,
  3;28,25,10;,
  3;2,2,2;,
  3;24,59,9;,
  3;32,40,42;,
  3;2,2,2;,
  3;2,2,2;,
  3;37,60,3;,
  3;11,58,19;,
  3;2,2,2;,
  3;22,30,9;,
  3;2,2,2;,
  3;19,57,5;,
  3;29,16,10;,
  3;2,2,2;,
  3;2,2,2;,
  3;23,1,59;,
  3;31,28,61;,
  3;6,15,22;,
  3;18,7,21;,
  3;15,62,30;,
  3;18,29,25;,
  3;20,28,31;,
  3;20,8,25;,
  3;63,24,30;,
  3;33,6,26;,
  3;64,23,24;,
  3;40,35,4;,
  3;32,65,41;,
  3;40,31,0;,
  3;27,12,60;,
  3;38,26,66;,
  3;67,68,56;,
  3;11,67,58;,
  3;48,45,46;,
  3;49,44,14;,
  3;13,45,48;,
  3;46,44,49;,
  3;69,48,51;,
  3;47,49,52;,
  3;51,48,47;,
  3;52,49,34;,
  3;52,70,50;,
  3;51,50,71;,
  3;52,72,70;,
  3;73,51,71;,
  3;54,55,46;,
  3;53,74,14;,
  3;13,36,54;,
  3;46,55,53;,
  3;57,58,55;,
  3;56,68,74;,
  3;36,5,57;,
  3;55,58,56;,
  3;33,38,27;,
  3;75,76,27;,
  3;7,75,37;,
  3;77,78,32;,
  3;32,78,35;,
  3;77,39,23;,
  3;50,70,11;,
  3;71,50,11;,
  3;19,58,57;,
  3;58,67,56;,
  3;39,1,23;,
  3;26,17,66;,
  3;27,43,12;,
  3;21,16,29;,
  3;2,2,2;,
  3;2,2,2;,
  3;28,10,61;,
  3;2,2,2;,
  3;24,9,30;,
  3;32,42,65;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;37,3,21;,
  3;2,2,2;,
  3;22,9,17;,
  3;2,2,2;,
  3;29,10,25;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;23,59,24;,
  3;31,61,0;,
  3;6,22,26;,
  3;18,21,29;,
  3;15,30,22;,
  3;18,25,8;,
  3;20,31,4;,
  3;20,25,28;,
  3;63,30,62;,
  3;33,26,38;,
  3;64,24,63;,
  3;40,4,31;,
  3;32,41,39;,
  3;40,0,42;,
  3;27,60,37;,
  3;38,66,43;,
  3;48,46,47;,
  3;49,14,34;,
  3;13,48,69;,
  3;46,49,47;,
  3;69,51,73;,
  3;47,52,50;,
  3;51,47,50;,
  3;52,34,72;,
  3;54,46,45;,
  3;53,14,44;,
  3;13,54,45;,
  3;46,53,44;,
  3;57,55,54;,
  3;56,74,53;,
  3;36,57,54;,
  3;55,56,53;,
  3;33,27,76;,
  3;75,27,37;,
  3;7,37,21;,
  3;77,32,39;,
  3;32,35,40;,
  3;77,23,64;;
 }
 MeshTextureCoords {
  104;
  0.221450;0.467400;,
  0.218370;0.408390;,
  0.317050;0.413250;,
  0.665620;0.473100;,
  0.567430;0.481620;,
  0.580100;0.417780;,
  0.910780;0.462040;,
  0.777800;0.467400;,
  0.780890;0.408390;,
  0.360710;0.106220;,
  0.327120;0.044270;,
  0.419070;0.025680;,
  0.042050;0.761460;,
  0.083980;0.716930;,
  0.143320;0.818710;,
  0.565160;0.091110;,
  0.501070;0.090580;,
  0.500450;0.018110;,
  0.352000;0.675840;,
  0.371300;0.813370;,
  0.257310;0.816040;,
  0.431820;0.481620;,
  0.419150;0.417780;,
  0.499630;0.422050;,
  0.920210;0.183100;,
  0.722760;0.124780;,
  0.753670;0.062130;,
  0.330310;0.941030;,
  0.252330;0.948510;,
  0.104020;0.922690;,
  0.065230;0.895410;,
  0.280020;0.128630;,
  0.248020;0.066520;,
  0.751250;0.668680;,
  0.754040;0.771220;,
  0.675370;0.657080;,
  0.173010;0.680040;,
  0.499630;0.482810;,
  0.174760;0.944680;,
  0.641580;0.772490;,
  0.607860;0.693500;,
  0.437010;0.092220;,
  0.438400;0.708620;,
  0.443750;0.886530;,
  0.486280;0.810670;,
  0.333640;0.473100;,
  0.641690;0.103790;,
  0.581940;0.024270;,
  0.654770;0.591210;,
  0.563580;0.598090;,
  0.442170;0.250340;,
  0.372400;0.252220;,
  0.499630;0.615930;,
  0.562740;0.249290;,
  0.502470;0.251730;,
  0.435670;0.598090;,
  0.780480;0.587330;,
  0.344480;0.591210;,
  0.708340;0.267960;,
  0.632540;0.249960;,
  0.088480;0.462030;,
  0.091070;0.403680;,
  0.674200;0.041250;,
  0.083610;0.190360;,
  0.064310;0.124000;,
  0.682210;0.413260;,
  0.826470;0.656250;,
  0.895940;0.691920;,
  0.866540;0.771260;,
  0.642600;0.854040;,
  0.641500;0.827190;,
  0.755570;0.827600;,
  0.869950;0.852800;,
  0.869590;0.825940;,
  0.962400;0.823850;,
  0.548610;0.826120;,
  0.589160;0.901190;,
  0.644570;0.878180;,
  0.756330;0.855420;,
  0.869300;0.876950;,
  0.925940;0.899350;,
  0.815620;0.940530;,
  0.756940;0.877880;,
  0.701690;0.941150;,
  0.875550;0.925390;,
  0.640950;0.926670;,
  0.641060;0.799070;,
  0.754760;0.797720;,
  0.868500;0.797820;,
  0.950600;0.753070;,
  0.556550;0.755220;,
  0.296920;0.271530;,
  0.132320;0.324690;,
  0.218770;0.587320;,
  0.085060;0.582570;,
  0.873840;0.318260;,
  0.758920;0.950600;,
  0.908180;0.403690;,
  0.028330;0.821410;,
  0.263090;0.662410;,
  0.938350;0.116420;,
  0.477050;0.751260;,
  0.402840;0.915680;,
  0.914190;0.582580;;
 }
}
