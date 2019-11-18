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
 115;
 -0.07455;-0.80512;0.12835;,
 -0.07455;-0.07090;-1.23265;,
 1.28960;-1.53052;-1.37470;,
 1.22700;-0.74007;-2.07982;,
 2.40237;-0.41127;-1.70057;,
 3.23878;0.86505;-1.32850;,
 3.19703;-0.09557;-1.03735;,
 2.40237;-0.41127;-1.70057;,
 3.19703;-0.09557;-1.03735;,
 1.98950;-1.04950;-0.94720;,
 3.62630;-0.96408;-1.20160;,
 3.89337;-1.33773;-1.44372;,
 3.87050;-0.97887;-1.88142;,
 3.60450;-0.70947;-1.54065;,
 5.17567;-0.00347;-0.82447;,
 6.01605;-0.15333;-1.06142;,
 5.89797;0.13062;1.12903;,
 3.23878;0.83457;1.37620;,
 3.60450;-0.70947;-1.54065;,
 3.87050;-0.97887;-1.88142;,
 3.89337;-1.33773;-1.44372;,
 -0.07455;-0.21925;1.09660;,
 -0.07455;-0.80512;0.12835;,
 1.59952;-0.10278;1.36970;,
 1.28960;-1.53052;-1.37470;,
 3.23878;0.13460;1.42730;,
 1.49997;0.82117;1.44438;,
 -0.07455;-0.21925;1.09660;,
 1.59952;-0.10278;1.36970;,
 3.23878;0.13460;1.42730;,
 6.01605;-0.15333;-1.06142;,
 5.63588;-0.19155;1.18013;,
 5.63588;-0.19155;1.18013;,
 -0.07455;0.86460;0.42700;,
 -0.07455;0.86460;-0.56747;,
 -0.07365;-0.80560;0.12743;,
 0.00000;0.92323;-0.01070;,
 0.35669;0.85455;-0.33032;,
 0.00000;0.85455;-0.46271;,
 0.00000;0.92323;-0.01070;,
 0.50443;0.85455;-0.01070;,
 0.00000;0.92323;-0.01070;,
 0.35669;0.85455;0.30892;,
 0.00000;0.92323;-0.01070;,
 -0.00000;0.85455;0.44132;,
 0.00000;0.92323;-0.01070;,
 -0.35669;0.85455;0.30892;,
 0.00000;0.92323;-0.01070;,
 -0.50443;0.85455;-0.01070;,
 0.00000;0.92323;-0.01070;,
 -0.35669;0.85455;-0.33032;,
 0.00000;0.92323;-0.01070;,
 0.00000;0.85455;-0.46271;,
 0.65907;0.65896;-0.60128;,
 0.00000;0.65896;-0.84590;,
 0.93207;0.65896;-0.01070;,
 0.65907;0.65896;0.57989;,
 -0.00000;0.65896;0.82451;,
 -0.65907;0.65896;0.57989;,
 -0.93207;0.65896;-0.01070;,
 -0.65907;0.65896;-0.60128;,
 0.00000;0.65896;-0.84590;,
 0.86112;0.36624;-0.78233;,
 0.00000;0.36624;-1.10195;,
 1.21781;0.36624;-0.01070;,
 0.86112;0.36624;0.76094;,
 -0.00000;0.36624;1.08056;,
 -0.86112;0.36624;0.76094;,
 -1.21781;0.36624;-0.01070;,
 -0.86112;0.36624;-0.78233;,
 0.00000;0.36624;-1.10195;,
 0.93207;0.02095;-0.84590;,
 0.00000;0.02095;-1.19186;,
 1.31814;0.02095;-0.01070;,
 0.93207;0.02095;0.82451;,
 -0.00000;0.02095;1.17047;,
 -0.93207;0.02095;0.82451;,
 -1.31814;0.02095;-0.01070;,
 -0.93207;0.02095;-0.84590;,
 0.00000;0.02095;-1.19186;,
 0.86112;-0.32434;-0.78233;,
 0.00000;-0.32434;-1.10195;,
 1.21781;-0.32434;-0.01070;,
 0.86112;-0.32434;0.76094;,
 -0.00000;-0.32434;1.08056;,
 -0.86112;-0.32434;0.76094;,
 -1.21781;-0.32434;-0.01070;,
 -0.86112;-0.32434;-0.78233;,
 0.00000;-0.32434;-1.10195;,
 0.65907;-0.61706;-0.60128;,
 0.00000;-0.61706;-0.84590;,
 0.93207;-0.61706;-0.01070;,
 0.65907;-0.61706;0.57989;,
 -0.00000;-0.61706;0.82451;,
 -0.65907;-0.61706;0.57989;,
 -0.93207;-0.61706;-0.01070;,
 -0.65907;-0.61706;-0.60128;,
 0.00000;-0.61706;-0.84590;,
 0.35669;-0.81265;-0.33032;,
 0.00000;-0.81265;-0.46271;,
 0.50443;-0.81265;-0.01070;,
 0.35669;-0.81265;0.30892;,
 -0.00000;-0.81265;0.44132;,
 -0.35669;-0.81265;0.30892;,
 -0.50443;-0.81265;-0.01070;,
 -0.35669;-0.81265;-0.33032;,
 0.00000;-0.81265;-0.46271;,
 0.00000;-0.88133;-0.01070;,
 0.00000;-0.88133;-0.01070;,
 0.00000;-0.88133;-0.01070;,
 0.00000;-0.88133;-0.01070;,
 0.00000;-0.88133;-0.01070;,
 0.00000;-0.88133;-0.01070;,
 0.00000;-0.88133;-0.01070;,
 0.00000;-0.88133;-0.01070;;
 
 105;
 3;0,1,2;,
 3;2,1,3;,
 3;4,5,6;,
 3;7,8,9;,
 3;10,11,12;,
 3;10,12,13;,
 3;10,13,7;,
 3;10,7,9;,
 3;5,14,15;,
 3;5,15,6;,
 3;16,14,5;,
 3;16,5,17;,
 3;4,18,19;,
 3;19,3,4;,
 3;2,3,19;,
 3;2,19,20;,
 3;21,22,23;,
 3;22,24,23;,
 3;23,8,25;,
 3;26,27,28;,
 3;26,29,17;,
 3;26,28,29;,
 3;30,14,16;,
 3;30,16,31;,
 3;23,24,9;,
 3;23,9,8;,
 3;29,32,16;,
 3;29,16,17;,
 3;25,8,30;,
 3;25,30,31;,
 3;11,10,9;,
 3;24,11,9;,
 3;33,27,26;,
 3;34,33,26;,
 3;5,34,26;,
 3;5,26,17;,
 3;3,5,4;,
 3;34,5,3;,
 3;34,3,1;,
 4;34,0,27,33;,
 4;35,0,34,1;,
 3;36,37,38;,
 3;39,40,37;,
 3;41,42,40;,
 3;43,44,42;,
 3;45,46,44;,
 3;47,48,46;,
 3;49,50,48;,
 3;51,52,50;,
 4;38,37,53,54;,
 4;37,40,55,53;,
 4;40,42,56,55;,
 4;42,44,57,56;,
 4;44,46,58,57;,
 4;46,48,59,58;,
 4;48,50,60,59;,
 4;50,52,61,60;,
 4;54,53,62,63;,
 4;53,55,64,62;,
 4;55,56,65,64;,
 4;56,57,66,65;,
 4;57,58,67,66;,
 4;58,59,68,67;,
 4;59,60,69,68;,
 4;60,61,70,69;,
 4;63,62,71,72;,
 4;62,64,73,71;,
 4;64,65,74,73;,
 4;65,66,75,74;,
 4;66,67,76,75;,
 4;67,68,77,76;,
 4;68,69,78,77;,
 4;69,70,79,78;,
 4;72,71,80,81;,
 4;71,73,82,80;,
 4;73,74,83,82;,
 4;74,75,84,83;,
 4;75,76,85,84;,
 4;76,77,86,85;,
 4;77,78,87,86;,
 4;78,79,88,87;,
 4;81,80,89,90;,
 4;80,82,91,89;,
 4;82,83,92,91;,
 4;83,84,93,92;,
 4;84,85,94,93;,
 4;85,86,95,94;,
 4;86,87,96,95;,
 4;87,88,97,96;,
 4;90,89,98,99;,
 4;89,91,100,98;,
 4;91,92,101,100;,
 4;92,93,102,101;,
 4;93,94,103,102;,
 4;94,95,104,103;,
 4;95,96,105,104;,
 4;96,97,106,105;,
 3;99,98,107;,
 3;98,100,108;,
 3;100,101,109;,
 3;101,102,110;,
 3;102,103,111;,
 3;103,104,112;,
 3;104,105,113;,
 3;105,106,114;;
 
 MeshMaterialList {
  2;
  105;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.500000;0.500000;0.500000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.300000;0.300000;0.300000;;
   TextureFilename {
    "data\\TEXTURE\\tex.jpg";
   }
  }
  Material {
   0.476863;0.476863;0.476863;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  85;
  -0.544566;0.711880;-0.443479;,
  -0.760895;-0.223851;-0.609040;,
  -0.582481;-0.172202;0.794394;,
  -0.721995;-0.686455;0.086624;,
  -0.659987;0.668535;0.342750;,
  0.287992;0.803256;-0.521384;,
  -0.166362;-0.976396;-0.137745;,
  0.135733;0.717599;0.683102;,
  0.418480;-0.856144;-0.303137;,
  -0.137870;0.125977;-0.982406;,
  -0.115498;0.661871;0.740667;,
  0.303240;0.952452;0.029666;,
  0.532730;0.674763;0.510778;,
  0.676229;0.695213;0.243706;,
  0.501591;0.130866;-0.855149;,
  0.393752;-0.793728;0.463632;,
  0.496878;0.850663;0.171711;,
  0.499048;-0.674018;0.544657;,
  0.519055;0.500356;0.692983;,
  0.686837;0.272995;-0.673594;,
  0.150560;-0.749859;0.644239;,
  0.043800;-0.567474;0.822226;,
  0.372212;-0.761163;0.531121;,
  -0.936828;-0.134798;-0.322774;,
  0.092033;0.771741;0.629243;,
  0.064610;0.743673;0.665414;,
  -0.070303;-0.926959;-0.368517;,
  -0.000000;1.000000;-0.000000;,
  -0.000000;0.952224;-0.305400;,
  0.195302;0.956484;-0.216783;,
  0.277267;0.960793;0.000000;,
  0.195302;0.956484;0.216783;,
  -0.000000;0.952224;0.305400;,
  -0.195302;0.956484;0.216783;,
  -0.277267;0.960793;-0.000000;,
  -0.195302;0.956484;-0.216783;,
  -0.000000;0.790131;-0.612938;,
  0.399406;0.804580;-0.439461;,
  0.572926;0.819607;0.000000;,
  0.399406;0.804580;0.439461;,
  -0.000000;0.790131;0.612938;,
  -0.399406;0.804580;0.439461;,
  -0.572926;0.819607;0.000000;,
  -0.399406;0.804580;-0.439461;,
  -0.000000;0.470120;-0.882603;,
  0.590462;0.488227;-0.642642;,
  0.861378;0.507964;0.000000;,
  0.590462;0.488227;0.642642;,
  -0.000000;0.470120;0.882603;,
  -0.590462;0.488227;0.642642;,
  -0.861378;0.507964;0.000000;,
  -0.590462;0.488227;-0.642642;,
  -0.000000;-0.000000;-1.000000;,
  0.678440;-0.000000;-0.734656;,
  1.000000;0.000000;0.000000;,
  0.678440;0.000000;0.734655;,
  -0.000000;-0.000000;1.000000;,
  -0.678441;-0.000000;0.734655;,
  -1.000000;-0.000000;0.000000;,
  -0.678440;-0.000000;-0.734655;,
  -0.000000;-0.470120;-0.882603;,
  0.590462;-0.488227;-0.642642;,
  0.861378;-0.507965;0.000000;,
  0.590462;-0.488227;0.642642;,
  -0.000000;-0.470120;0.882603;,
  -0.590462;-0.488227;0.642642;,
  -0.861378;-0.507965;0.000000;,
  -0.590462;-0.488227;-0.642642;,
  -0.000000;-0.790131;-0.612938;,
  0.399406;-0.804580;-0.439461;,
  0.572926;-0.819607;-0.000000;,
  0.399406;-0.804580;0.439461;,
  -0.000000;-0.790131;0.612938;,
  -0.399406;-0.804580;0.439461;,
  -0.572926;-0.819607;0.000000;,
  -0.399406;-0.804580;-0.439461;,
  0.000000;-0.952224;-0.305401;,
  0.195302;-0.956484;-0.216783;,
  0.277267;-0.960793;0.000000;,
  0.195302;-0.956484;0.216783;,
  -0.000000;-0.952224;0.305401;,
  -0.195302;-0.956484;0.216783;,
  -0.277267;-0.960793;0.000000;,
  -0.195302;-0.956484;-0.216783;,
  0.000000;-1.000000;-0.000000;;
  105;
  3;3,1,6;,
  3;6,1,9;,
  3;14,5,8;,
  3;14,8,15;,
  3;18,17,19;,
  3;18,19,16;,
  3;18,16,24;,
  3;18,24,25;,
  3;5,11,13;,
  3;5,26,8;,
  3;12,11,5;,
  3;12,5,7;,
  3;14,16,19;,
  3;19,9,14;,
  3;6,9,19;,
  3;6,19,17;,
  3;2,3,20;,
  3;3,6,20;,
  3;20,8,21;,
  3;10,2,20;,
  3;10,21,7;,
  3;10,20,21;,
  3;13,11,12;,
  3;13,12,22;,
  3;20,6,15;,
  3;20,15,8;,
  3;21,22,12;,
  3;21,12,7;,
  3;21,8,26;,
  3;21,26,22;,
  3;17,18,15;,
  3;6,17,15;,
  3;4,2,10;,
  3;0,4,10;,
  3;5,0,10;,
  3;5,10,7;,
  3;9,5,14;,
  3;0,5,9;,
  3;0,9,1;,
  4;0,3,2,4;,
  4;23,3,0,1;,
  3;27,29,28;,
  3;27,30,29;,
  3;27,31,30;,
  3;27,32,31;,
  3;27,33,32;,
  3;27,34,33;,
  3;27,35,34;,
  3;27,28,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,28,36,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,36,44,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,50,58,57;,
  4;50,51,59,58;,
  4;51,44,52,59;,
  4;52,53,61,60;,
  4;53,54,62,61;,
  4;54,55,63,62;,
  4;55,56,64,63;,
  4;56,57,65,64;,
  4;57,58,66,65;,
  4;58,59,67,66;,
  4;59,52,60,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,67,75,74;,
  4;67,60,68,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,74,82,81;,
  4;74,75,83,82;,
  4;75,68,76,83;,
  3;76,77,84;,
  3;77,78,84;,
  3;78,79,84;,
  3;79,80,84;,
  3;80,81,84;,
  3;81,82,84;,
  3;82,83,84;,
  3;83,76,84;;
 }
 MeshTextureCoords {
  115;
  0.382500;0.945620;,
  0.412920;0.919980;,
  0.433580;0.967180;,
  0.442230;0.945400;,
  0.471380;0.942760;,
  0.494500;0.911460;,
  0.500350;0.936340;,
  0.674530;0.894700;,
  0.649420;0.882600;,
  0.687380;0.874720;,
  0.688210;0.923860;,
  0.696120;0.930420;,
  0.687810;0.936560;,
  0.680270;0.929520;,
  0.570500;0.907310;,
  0.570650;0.915920;,
  0.562370;0.852040;,
  0.495550;0.846440;,
  0.496370;0.957850;,
  0.496750;0.969750;,
  0.493060;0.982560;,
  0.710800;0.799050;,
  0.723710;0.824940;,
  0.669130;0.811420;,
  0.711490;0.878680;,
  0.628350;0.822920;,
  0.455100;0.844930;,
  0.411210;0.835220;,
  0.453840;0.822880;,
  0.499300;0.829570;,
  0.580140;0.905640;,
  0.570930;0.848830;,
  0.560150;0.842370;,
  0.416790;0.866770;,
  0.416690;0.891400;,
  0.000000;0.000000;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
