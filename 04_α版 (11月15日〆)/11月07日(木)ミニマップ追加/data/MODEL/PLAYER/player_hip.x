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
 59;
 2.42657;-0.11216;-1.89602;,
 0.00000;-0.27620;0.34648;,
 -0.01572;-1.19875;1.13725;,
 3.19304;-1.11591;-1.80132;,
 0.00000;-0.27620;0.34648;,
 -2.42657;-0.11216;-1.89602;,
 -0.01572;-1.19875;1.13725;,
 -3.20476;-1.11591;-1.80132;,
 4.92790;-2.52552;-1.74030;,
 2.78886;-2.56787;0.94228;,
 -0.04865;-2.39846;3.47803;,
 -3.06078;-2.82199;0.66053;,
 -4.93790;-2.52552;-1.74030;,
 0.00000;-2.09793;-4.40941;,
 3.42700;-2.14683;-1.66602;,
 2.08177;-2.96584;-4.13114;,
 -3.42700;-2.14683;-1.66602;,
 0.00000;-2.09793;-4.40941;,
 -2.08177;-2.96584;-4.13114;,
 3.42700;-2.14683;-1.66602;,
 0.00000;-2.10333;0.70693;,
 -3.42700;-2.14683;-1.66602;,
 -0.22327;-2.96009;-5.79194;,
 -0.01572;-1.19875;-4.95739;,
 3.06084;-2.87371;-4.31250;,
 3.19304;-1.11591;-1.80132;,
 4.92790;-2.52552;-1.74030;,
 -4.93790;-2.52552;-1.74030;,
 -3.20476;-1.11591;-1.80132;,
 -3.05983;-2.87371;-4.31250;,
 -0.01572;-1.19875;-4.95739;,
 -2.42657;-0.11216;-1.89602;,
 0.00000;-0.27620;-4.25838;,
 2.42657;-0.11216;-1.89602;,
 3.19304;-1.11591;-1.80132;,
 -0.00939;-0.07842;-4.77877;,
 -0.01767;-0.95720;-5.36927;,
 3.49748;-0.84491;-1.79372;,
 2.85926;0.01385;-1.75189;,
 -2.86005;0.01258;-1.75244;,
 -3.51332;-0.84779;-1.78388;,
 -0.00939;-0.07842;-4.77877;,
 0.00879;-0.11585;0.88299;,
 -0.02094;-0.96972;1.56505;,
 0.00879;-0.11585;0.88299;,
 -0.02094;-0.96972;1.56505;,
 -3.51332;-0.84779;-1.78388;,
 5.35597;-2.37410;-1.71574;,
 2.98954;-2.76971;1.08263;,
 -0.04558;-2.32831;3.74968;,
 -3.26330;-2.92564;0.93450;,
 -5.37185;-2.39118;-1.73409;,
 -0.01767;-0.95720;-5.36927;,
 -0.21343;-2.85832;-6.21041;,
 3.06720;-2.87371;-4.31250;,
 3.49748;-0.84491;-1.79372;,
 -3.06916;-2.87371;-4.31250;,
 5.35597;-2.37410;-1.71574;,
 -5.37185;-2.39118;-1.73409;;
 
 52;
 3;0,1,2;,
 3;3,0,2;,
 3;4,5,6;,
 3;5,7,6;,
 3;8,3,9;,
 3;2,10,9;,
 3;3,2,9;,
 3;11,7,12;,
 3;11,10,6;,
 3;6,7,11;,
 3;13,14,15;,
 3;16,17,18;,
 3;19,8,9;,
 3;20,19,9;,
 3;20,9,10;,
 3;21,11,12;,
 3;11,21,20;,
 3;20,10,11;,
 3;22,23,24;,
 3;23,25,24;,
 3;25,26,24;,
 3;27,28,29;,
 3;28,30,29;,
 3;22,29,30;,
 3;26,14,24;,
 3;14,13,24;,
 3;13,22,24;,
 3;22,17,29;,
 3;17,16,29;,
 3;16,27,29;,
 3;31,32,30;,
 3;28,31,30;,
 3;32,33,30;,
 3;33,34,30;,
 4;35,36,37,38;,
 4;39,40,36,41;,
 3;42,38,43;,
 3;38,37,43;,
 3;39,44,45;,
 3;46,39,45;,
 3;37,47,48;,
 3;49,43,48;,
 3;43,37,48;,
 3;46,50,51;,
 3;49,50,45;,
 3;46,45,50;,
 3;52,53,54;,
 3;55,52,54;,
 3;36,40,56;,
 3;56,53,36;,
 3;57,55,54;,
 3;40,58,56;;
 
 MeshMaterialList {
  1;
  52;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "C:\\Users\\game114\\Desktop\\2年生後期_3Dスクロールアクション\\素材\\MODEL\\PLAYER\\Nejiko.png";
   }
  }
 }
 MeshNormals {
  64;
  -0.017400;0.429792;-0.902760;,
  -0.004291;0.604985;-0.796225;,
  0.488511;0.592760;-0.640307;,
  0.630932;0.775447;-0.024631;,
  0.038589;-0.983004;-0.179482;,
  0.000000;0.856060;-0.516876;,
  0.391909;0.787579;-0.475529;,
  0.521222;0.668857;0.530054;,
  -0.588943;0.494429;-0.639286;,
  -0.495889;0.609294;-0.618752;,
  -0.391909;0.787579;-0.475528;,
  -0.494066;0.728368;0.474741;,
  -0.534944;0.679465;0.502158;,
  -0.528725;0.638680;0.559050;,
  -0.032359;0.889642;0.455511;,
  0.464056;0.763499;0.449134;,
  0.330338;0.734205;0.593144;,
  0.540592;-0.537325;0.647334;,
  0.019773;-0.406235;0.913555;,
  -0.532581;-0.515590;0.671212;,
  -0.565822;-0.462081;0.682881;,
  -0.631816;-0.459412;0.624298;,
  -0.522913;-0.662847;-0.535908;,
  -0.576454;-0.497638;-0.648118;,
  -0.577508;-0.557351;0.596527;,
  0.520076;-0.680884;-0.515673;,
  0.526366;-0.651469;-0.546377;,
  0.577887;-0.558799;0.594803;,
  0.631984;-0.465062;0.619930;,
  -0.509000;-0.728363;-0.458701;,
  -0.509068;-0.691233;-0.512881;,
  -0.555907;-0.590461;-0.585084;,
  0.552721;0.536902;0.637366;,
  -0.561680;0.529182;0.635989;,
  0.565099;0.519351;0.641044;,
  -0.546241;0.550475;0.631346;,
  -0.542166;0.623060;0.563784;,
  -0.221801;-0.974115;0.043640;,
  -0.242707;-0.969362;0.037813;,
  -0.183382;-0.967988;-0.171376;,
  0.037872;-0.993733;-0.105170;,
  0.243766;-0.969792;-0.009077;,
  0.233367;-0.942971;-0.237372;,
  0.243567;-0.969833;-0.009958;,
  0.566573;0.603005;-0.561588;,
  -0.557900;0.664421;-0.497284;,
  -0.567039;0.603552;-0.560528;,
  -0.231196;-0.897573;0.375381;,
  0.001669;-0.909109;0.416556;,
  0.003268;-0.848761;0.528766;,
  0.232090;-0.905110;0.356243;,
  -0.610087;0.461323;-0.644186;,
  0.595648;0.478624;-0.645076;,
  0.613587;0.449644;-0.649101;,
  0.575193;-0.486035;0.657969;,
  -0.597108;-0.462273;-0.655565;,
  -0.549777;-0.595514;-0.585754;,
  0.587976;-0.485527;-0.646953;,
  0.561450;-0.528306;-0.636919;,
  0.557225;-0.595586;-0.578600;,
  0.552812;-0.589341;-0.589132;,
  0.524389;-0.725702;-0.445390;,
  -0.529775;-0.614024;0.585075;,
  0.526982;-0.615686;0.585850;;
  52;
  3;34,32,16;,
  3;7,34,16;,
  3;35,33,36;,
  3;33,13,36;,
  3;3,7,15;,
  3;16,14,15;,
  3;7,16,15;,
  3;11,13,12;,
  3;11,14,36;,
  3;16,13,11;,
  3;5,6,6;,
  3;10,5,10;,
  3;37,38,39;,
  3;4,37,39;,
  3;4,39,40;,
  3;41,42,43;,
  3;42,41,4;,
  3;4,40,42;,
  3;0,1,2;,
  3;1,44,2;,
  3;44,3,2;,
  3;45,46,9;,
  3;46,1,9;,
  3;0,9,1;,
  3;38,37,47;,
  3;37,48,47;,
  3;48,49,47;,
  3;49,48,50;,
  3;48,41,50;,
  3;41,43,50;,
  3;51,8,1;,
  3;46,51,1;,
  3;52,53,1;,
  3;53,44,1;,
  4;21,20,24,21;,
  4;28,27,54,28;,
  3;23,55,31;,
  3;55,56,31;,
  3;57,58,59;,
  3;60,57,59;,
  3;56,22,30;,
  3;29,31,30;,
  3;31,56,30;,
  3;60,25,26;,
  3;61,25,59;,
  3;60,59,25;,
  3;20,18,19;,
  3;24,20,19;,
  3;54,27,17;,
  3;17,18,54;,
  3;62,24,19;,
  3;27,63,17;;
 }
 MeshTextureCoords {
  59;
  0.399555;0.334060;,
  0.306611;0.337165;,
  0.301611;0.444769;,
  0.471595;0.437756;,
  0.306575;0.337165;,
  0.399518;0.334060;,
  0.301575;0.444769;,
  0.471559;0.437756;,
  0.588867;0.521853;,
  0.464946;0.542752;,
  0.302037;0.541039;,
  0.464946;0.542752;,
  0.588831;0.521853;,
  0.284513;0.580230;,
  0.133252;0.581826;,
  0.190861;0.621299;,
  0.133252;0.581826;,
  0.284513;0.579969;,
  0.190861;0.621299;,
  0.459474;0.581826;,
  0.312713;0.579730;,
  0.459474;0.581826;,
  0.293339;0.541252;,
  0.286115;0.443769;,
  0.133205;0.540566;,
  0.118631;0.438256;,
  0.012359;0.527353;,
  0.012395;0.527353;,
  0.118667;0.438256;,
  0.133205;0.540566;,
  0.286151;0.443769;,
  0.170171;0.331060;,
  0.286151;0.337166;,
  0.170208;0.331060;,
  0.118667;0.438256;,
  0.046611;0.954287;,
  0.046615;0.958551;,
  0.065159;0.958310;,
  0.060505;0.954042;,
  0.060502;0.954042;,
  0.030117;0.958310;,
  0.046615;0.954287;,
  0.048661;0.954287;,
  0.048661;0.958551;,
  0.048657;0.954287;,
  0.048657;0.958551;,
  0.065156;0.958310;,
  0.076487;0.961414;,
  0.065895;0.962190;,
  0.048904;0.961082;,
  0.065895;0.962190;,
  0.076483;0.961414;,
  0.046611;0.958551;,
  0.045384;0.962190;,
  0.029871;0.962523;,
  0.030113;0.958310;,
  0.029871;0.962523;,
  0.018786;0.961414;,
  0.018790;0.961414;;
 }
}
