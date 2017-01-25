/*
LICENSE
	This software is dual-licensed to the public domain and under the following
	license: you are granted a perpetual, irrevocable license to copy, modify,
	publish, and distribute this file as you see fit.

CREDITS
	Written by Gorevoy Dmitry
*/

#ifndef GD_WAVEFRONT_OBJ
#define GD_WAVEFRONT_OBJ

#ifndef GDWO_DEF
#ifdef GD_WAVEFRONT_OBJ_STATIC
#define GDWO_DEF static
#else
#define GDWO_DEF extern
#endif
#endif

#define GDWO_ASSERT(Expression) if(!(Expression)){*((int*)0) = 0;}
#define GDWO_IS_SPACE(OurChar) (((OurChar) == ' ') || ((OurChar) == '\t'))
#if 0
#define GDWO_IS_DIGIT(OurChar) (((OurChar >= '0')) && ((OurChar) <= '9')))
#else
#define GDWO_IS_DIGIT(OurChar) ((unsigned int)((OurChar) - '0') < (unsigned int)(10))
#endif
#define GDWO_IS_NEW_LINE(OurChar) (((OurChar) == '\r') || ((OurChar) == '\n') || ((OurChar) == '\0'))

#define GDWO_INVALID_INDEX (0x80000000)

#define GDWO_MAX_LINE_LEN 4096
#define GDWO_MAX_FACES_PER_LINE 16

#define GDWO_ERROR_SUCCESS (0)
#define GDWO_ERROR_EMPTY (-1)
#define GDWO_ERROR_INVALID_PARAMETER (-2)
#define GDWO_ERROR_FILE_OPERATION (-3)

#define GDWO_FLAG_TRIANGULATE 1

typedef enum gdwoTokenType{
	TokenType_Empty,
	TokenType_V,
	TokenType_VN,
	TokenType_VT,
	TokenType_F,
	TokenType_G,
	TokenType_O,
	TokenType_USEMTL,
	TokenType_MTLLIB
} gdwoTokenType;

typedef struct gdwoParseResult{
	gdwoTokenType Type;

	float VertX, VertY, VertZ;
	float NormX, NormY, NormZ;
	float TexCoordX, TexCoordY;

	gdwoVertexIndices FaceIndices[GDWO_MAX_FACES_PER_LINE];
	int FaceIndicesCount;

	int NumberOfIndices[GDWO_MAX_FACES_PER_LINE];
	int NumberOfIndicesCount;

	const char* GroupName;
	unsigned int GroupNameLen;
	int Pad0;

	const char* ObjectName;
	unsigned int ObjectNameLen;
	int Pad1;

	const char* MaterialName;
	unsigned int MaterialNameLen;
	int Pad2;

	const char* MTLLibName;
	unsigned int MTLLibNameLen;
} gdwoParseResult;

typedef struct gdwoMaterial{
	char* Name;

	float Ambient[3];
	float Diffuse[3];
	float Specular[3];
	float Transmittance[3];
	float Emission[3];
	float Shininess;
	float RefractionCoefficient;
	float Transparency; /* 1 - opaque, 0 - fully transparent */
	int Illum;

	int Pad0;

	char* AmbientTextureName;			/* map_Ka */
	char* DiffuseTextureName;			/* map_Kd */
	char* SpecularTextureName;			/* map_Ks */
	char* SpecularHighlightTextureName;	/* map_Ns */
	char* BumpTextureName;				/* map_bump, bump */
	char* DisplacementTextureName;		/* disp */
	char* AlphaTextureName;				/* map_d */
}gdwoMaterial;

typedef struct gdwoVertexIndices{
	int VertexIndex;
	int TexCoordIndex;
	int NormalIndex;
} gdwoVertexIndices;

typedef struct gdwoMeshAttrib{
	unsigned int NumberOfVertices;
	unsigned int NumberOfNormals;
	unsigned int NumberOfTexCoords;
	unsigned int NumberOfFaces;
	unsigned int NumberOfFaceVertices;

	int Pad0;

	float* Vertices;
	float* Normals;
	float* TexCoords;
	gdwoVertexIndices* Faces;
	int* FaceNumVertices;

	int* MaterialsIDs;
}gdwoMeshAttrib;

typedef struct gdwoShape{
	char* Name;
	unsigned int FaceOffset;
	unsigned int Length;
}gdwoShape;

#ifdef __cplusplus
extern "C"{
#endif

	GDWO_DEF int gd_parse_obj(
		gdwoMeshAttrib* Attrib,
		gdwoShape** Shapes, int* NumberOfShapes,
		gdwoMaterial** MaterialsOut, int* NumberOfMaterialsOut,
		const char* Buffer, int BufferLen,
		unsigned int Flags);

	GDWO_DEF void gd_attrib_init(gdwoMeshAttrib* Attrib);
	GDWO_DEF void gd_attrib_free(gdwoMeshAttrib* Attrib);
	GDWO_DEF void gd_shapes_free(gdwoShape* Shapes, int NumberOfShapes);
	GDWO_DEF void gd_materials_free(gdwoMaterial* Materials, int NumberOfMaterials);

#ifdef __cplusplus
}
#endif

#endif

#if defined(GD_WAVEFRONT_OBJ_IMPLEMENTATION) && !defined(GD_WAVEFRONT_OBJ_IMPLEMENTATION_DONE)
#define GD_WAVEFRONT_OBJ_IMPLEMENTATION_DONE

static void gdwo__skip_space(const char** Token){
	while ((*Token)[0] == ' ' || (*Token)[0] == '\t'){
		(*Token)++;
	}
}

static void gdwo__skip_space_and_cr(const char** Token){
	while ((*Token)[0] == ' ' || (*Token)[0] == '\t' || (*Token)[0] == '\r'){
		(*Token)++;
	}
}

static int gdwo__until_space(const char* Token){
	const char* TokenCopy = Token;
	while (
		TokenCopy[0] != '\0' &&
		TokenCopy[0] != ' ' &&
		TokenCopy[0] != '\t' &&
		TokenCopy[0] != '\r')
	{
		TokenCopy++;
	}

	return((int)(TokenCopy - Token));
}


static int gdwo__length_until_new_line(const char* Token, int N){
	int Result = 0;

	for (Result = 0; Result < N - 1; Result++){
		if (Token[Result] == '\n'){
			break;
		}
		if ((Token[Result] == '\r') && ((Result < (N - 2)) && (Token[Result + 1] != '\n'))){
			break;
		}
	}

	return(Result);
}

static char* gdwo__strduplicate(const char* Text){
	char* Dup;
	int DupLen;

	if (Text == 0){
		return(0);
	}

	DupLen = strlen(Text);
	Dup = (char*)malloc(DupLen + 1);
	memcpy(Dup, Text, DupLen);
	Dup[DupLen] = 0;

	return(Dup);
}

static char* gdwo__strnduplicate(const char* Text, int Len){
	char* Dup;
	int DupLen;

	if (Text == 0){
		return(0);
	}
	if (Len == 0){
		return(0);
	}

	Dup = (char*)malloc(Len + 1);
	DupLen = strlen(Text);
	if (DupLen < Len){
		memcpy(Dup, Text, DupLen);
		Dup[DupLen] = 0;
	}
	else{
		memcpy(Dup, Text, Len);
		Dup[Len] = 0;
	}

	return(Dup);
}

/*
	Here we try to parse double from Text

	TextEnd should be a location in the string where reading should stop.
	For example the end of the string.

	Parses the following EBNF grammar:
		sign    = "+" | "-" ;
		END     = ? anything not in digit ?
		digit   = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
		integer = [sign] , digit , {digit} ;
		decimal = integer , ["." , integer] ;
		float   = ( decimal , END ) | ( decimal , ("E" | "e") , integer , END ) ;

	Valid strings for example are:
		-0  +3.1417e+2  -0.0E-3  1.0324  -1.41   11e2

	If the parsing is a success, result is set to the parsed value and true
	is returned.
	
	Text must be less than TextEnd
*/

static int gdwo__try_parse_double(const char* Text, const char* TextEnd, double* Result){
	double Mantissa = 0.0;

	int Exponent = 0;

	char Sign = '+';
	char ExpSign = '+';
	char const* Curr = Text;

	/*How many characters were read in the loop*/
	int CharsRead = 0;
	/*Is loop terminated by reaching the TextEnd*/
	int EndNotReached = 0;

	if (Text >= TextEnd){
		return 0;
	}

	if (*Curr == '+' || *Curr == '-'){
		Sign = *Curr;
		Curr++;
	}
	else if (GDWO_IS_DIGIT(*Curr)){

	}
	else{
		goto fail;
	}

	/*Read the integer part.*/
	EndNotReached = (Curr != TextEnd);
	while (EndNotReached && GDWO_IS_DIGIT(*Curr)){
		Mantissa *= 10;
		Mantissa += (int)(*Curr - 0x30);
		Curr++;
		CharsRead++;
		EndNotReached = (Curr != TextEnd);
	}

	/*Make sure we read something*/
	if (CharsRead == 0){
		goto fail;
	}

	if (!EndNotReached){
		goto assemble;
	}

	/*Read the decimal part*/
	if (*Curr == '.'){
		Curr++;
		CharsRead = 1;
		EndNotReached = (Curr != TextEnd);
		while (EndNotReached && GDWO_IS_DIGIT(*Curr)){
			double FractionalValue = 1.0;
			int F;
			for (F = 0; F < CharsRead; F++){
				FractionalValue *= 0.1;
			}
			Mantissa += (int)(*Curr - 0x30) * FractionalValue;
			CharsRead++;
			Curr++;
			EndNotReached = (Curr != TextEnd);
		}
	}
	else if (*Curr == 'e' || *Curr == 'E'){

	}
	else{
		goto assemble;
	}

	if (!EndNotReached){
		goto assemble;
	}

	if (*Curr == 'e' || *Curr == 'E'){
		Curr++;
		EndNotReached = (Curr != TextEnd);
		if (EndNotReached && (*Curr == '+' || *Curr == '-')){
			ExpSign = *Curr;
			Curr++;
		}
		else if (GDWO_IS_DIGIT(*Curr)){

		}
		else{
			goto fail;
		}

		CharsRead = 0;
		EndNotReached = (Curr != TextEnd);
		while(EndNotReached && GDWO_IS_DIGIT(*Curr)){
			Exponent *= 10;
			Exponent += (int)(*Curr - 0x30);
			Curr++;
			CharsRead++;
			EndNotReached = (Curr != TextEnd);
		}
		
		Exponent *= (ExpSign == '+' ? 1 : -1);
		if (CharsRead == 0){
			goto fail;
		}
	}

assemble:
	{
		double A = 1.0;
		int i;
		for (i = 0; i < Exponent; i++){
			A = A * 5.0;
		}
		*Result =
			(Sign == '+' ? 1 : -1) * (Mantissa * A) * (double)(1 << Exponent);
	}
	return(1);

fail:
	return 0;
}

static float gdwo__parse_float(const char** Token){
	const char* End;
	double DoubleValue = 0.0;
	float TempFloat = 0.0f;
	gdwo__skip_space(Token);
	End = (*Token) + gdwo__until_space((*Token));
	gdwo__try_parse_double((*Token), End, &DoubleValue);
	TempFloat = (float)(DoubleValue);
	(*Token) = End;
	return(TempFloat);
}

static void gdwo__parse_float2(float* X, float* Y, const char** Token){
	(*X) = gdwo__parse_float(Token);
	(*Y) = gdwo__parse_float(Token);
}

static void gdwo__parse_float3(float* X, float* Y, float* Z, const char** Token){
	(*X) = gdwo__parse_float(Token);
	(*Y) = gdwo__parse_float(Token);
	(*Z) = gdwo__parse_float(Token);
}

static int gdwo__atoi(const char* Text){
	int Result = 0;
	int Sign = 1;

	if (*Text == '+' || *Text == '-'){
		if (*Text == '-'){
			Sign = -1;
		}
		Text++;
	}
	while ((((*Text) >= '0') && ((*Text) <= '9'))){
		Result *= 10;
		Result += (int)(*Text - '0');
		Text++;
	}

	return(Result * Sign);
}

/*Here we parse diffrent types of triples: v, v/t/n, v//n, v/t*/
static gdwoVertexIndices gdwo__parse_triple(const char** Token){
	gdwoVertexIndices Result;

	Result.VertexIndex = GDWO_INVALID_INDEX;
	Result.TexCoordIndex = GDWO_INVALID_INDEX;
	Result.NormalIndex = GDWO_INVALID_INDEX;

	Result.VertexIndex = gdwo__atoi((*Token));
	while (
		(*Token)[0] != '\0' &&
		(*Token)[0] != '/' &&
		(*Token)[0] != ' ' &&
		(*Token)[0] != '\t' &&
		(*Token)[0] != '\r')
	{
		(*Token)++;
	}

	if ((*Token)[0] != '/'){
		return(Result);
	}
	(*Token)++;

	/*v//n*/
	if ((*Token)[0] == '/'){
		(*Token)++;
		Result.NormalIndex = gdwo__atoi((*Token));
		while (
			(*Token)[0] != '\0' &&
			(*Token)[0] != '/' &&
			(*Token)[0] != ' ' &&
			(*Token)[0] != '\t' &&
			(*Token)[0] != '\r')
		{
			(*Token)++;
		}
		return(Result);
	}

	/* v/t/n or v/t */
	Result.TexCoordIndex = gdwo__atoi((*Token));
	while (
		(*Token)[0] != '\0' &&
		(*Token)[0] != '/' &&
		(*Token)[0] != ' ' &&
		(*Token)[0] != '\t' &&
		(*Token)[0] != '\r')
	{
		(*Token)++;
	}
	if ((*Token)[0] != '/'){
		return(Result);
	}


	/* v/t/n */
	(*Token)++; /* Skipping '/' */
	Result.NormalIndex = gdwo__atoi((*Token));
	while (
		(*Token)[0] != '\0' &&
		(*Token)[0] != '/' &&
		(*Token)[0] != ' ' &&
		(*Token)[0] != '\t' &&
		(*Token)[0] != '\r')
	{
		(*Token)++;
	}

	return(Result);
}

static int gdwo__parse_int(const char** Token){
	int Result = 0;

	gdwo__skip_space(Token);
	Result = gdwo__atoi((*Token));
	(*Token) += gdwo__until_space((*Token));

	return(Result);
}

static int gdwo__parse_line(
	gdwoParseResult* Result,
	const char* Line,
	unsigned int LineLen,
	int Triangulate)
{

	char LineBuffer[GDWO_MAX_LINE_LEN];
	const char* Token;
	GDWO_ASSERT(LineLen < GDWO_MAX_LINE_LEN);

	memcpy(LineBuffer, Line, LineLen);
	LineBuffer[LineLen] = '\0';

	Token = LineBuffer;

	Result->Type = TokenType_Empty;

	gdwo__skip_space(&Token);
	GDWO_ASSERT(Token);

	/*Empty Line*/
	if (Token[0] == '\0'){
		return 0;
	}

	/*Comment Line*/
	if (Token[0] == '#'){
		return 0;
	}

	/*Vertex position*/
	if (Token[0] == 'v' && GDWO_IS_SPACE((Token[1]))){
		float X, Y, Z;
		Token += 2;
		gdwo__parse_float3(&X, &Y, &Z, &Token);
		Result->VertX = X;
		Result->VertY = Y;
		Result->VertZ = Z;
		Result->Type = TokenType_V;
		return(1);
	}

	/*Vertex Normal*/
	if (Token[0] == 'v' && Token[1] == 'n' && GDWO_IS_SPACE((Token[2]))){
		float X, Y, Z;
		Token += 3;
		gdwo__parse_float3(&X, &Y, &Z, &Token);
		Result->NormX = X;
		Result->NormY = Y;
		Result->NormZ = Z;
		Result->Type = TokenType_VN;
		return(1);
	}

	/*Texture Coordinates*/
	if (Token[0] == 'v' && Token[1] == 't' && GDWO_IS_SPACE((Token[2]))){
		float X, Y;
		Token += 3;
		gdwo__parse_float2(&X, &Y, &Token);
		Result->TexCoordX = X;
		Result->TexCoordY = Y;
		Result->Type = TokenType_VT;
		return(1);
	}

	/*Face*/
	if (Token[0] == 'f' && GDWO_IS_SPACE((Token[1]))){
		int NumberOfFaces = 0;
		
		gdwoVertexIndices Faces[GDWO_MAX_FACES_PER_LINE];

		Token += 2;
		gdwo__skip_space(&Token);

		while (!GDWO_IS_NEW_LINE(Token[0])){
			gdwoVertexIndices VertIndex = gdwo__parse_triple(&Token);
			gdwo__skip_space_and_cr(&Token);

			Faces[NumberOfFaces] = VertIndex;
			NumberOfFaces++;
		}

		Result->Type = TokenType_F;

		if (Triangulate){
			int K;
			int N = 0;

			gdwoVertexIndices Index0 = Faces[0];
			gdwoVertexIndices Index1;
			gdwoVertexIndices Index2 = Faces[1];

			/*???????????*/
			GDWO_ASSERT(3 * NumberOfFaces < GDWO_MAX_FACES_PER_LINE);

			for (K = 2; K < NumberOfFaces; K++){
				Index1 = Index2;
				Index2 = Faces[K];
				Result->FaceIndices[3 * N + 0] = Index0;
				Result->FaceIndices[3 * N + 1] = Index1;
				Result->FaceIndices[3 * N + 2] = Index2;

				Result->NumberOfIndices[N] = 3;
				N++;
			}
			Result->FaceIndicesCount = 3 * N;
			Result->NumberOfIndicesCount = N;
		}
		else{
			int K = 0;
			
			GDWO_ASSERT(NumberOfFaces < GDWO_MAX_FACES_PER_LINE);
			for (K = 0; K < NumberOfFaces; K++){
				Result->FaceIndices[K] = Faces[K];
			}

			Result->FaceIndicesCount = NumberOfFaces;
			Result->NumberOfIndices[0] = NumberOfFaces;
			Result->NumberOfIndicesCount = 1;
		}

		return(1);
	}

	/*Use MTL file*/
	if ((strncmp(Token, "usemtl", 6) == 0) && GDWO_IS_SPACE((Token[6]))){
		Token += 7;

		gdwo__skip_space(&Token);
		Result->MaterialName = Line + (Token - LineBuffer);
		Result->MaterialNameLen = (unsigned int)gdwo__length_until_new_line(Token, (LineLen - (Token - LineBuffer) + 1));
		Result->Type = TokenType_USEMTL;

		return(1);
	}

	/*Load MTL*/
	if ((strncmp(Token, "mtllib", 6) == 0) && GDWO_IS_SPACE((Token[6]))){
		Token += 7;

		gdwo__skip_space(&Token);
		Result->MTLLibName = Line + (Token - LineBuffer);
		Result->MTLLibNameLen = (unsigned int)gdwo__length_until_new_line(Token, LineLen - (Token - LineBuffer) + 1);
		Result->Type = TokenType_MTLLIB;

		return(1);
	}

	/*Group name*/
	if (Token[0] == 'g' && GDWO_IS_SPACE((Token[1]))){
		Token += 2;

		Result->GroupName = Line + (Token - LineBuffer);
		Result->GroupNameLen = (unsigned int)gdwo__length_until_new_line(Token, LineLen - (Token - LineBuffer) + 1);
		Result->Type = TokenType_G;

		return(1);
	}

	/*Object name*/
	if (Token[0] == 'o' && GDWO_IS_SPACE((Token[1]))){
		Token += 2;

		Result->ObjectName = Line + (Token - LineBuffer);
		Result->ObjectNameLen = (unsigned int)gdwo__length_until_new_line(Token, LineLen - (Token - LineBuffer) + 1);
		Result->Type = TokenType_O;

		return (1);
	}

	return(0);
}

static int gdwo__is_line_end(const char *Line, int Pos, int Len){
	if (Line[Pos] == '\0'){
		return(1);
	}
	if (Line[Pos] == '\n'){
		return(1);
	}
	if (Line[Pos] == '\r'){
		if (((Pos + 1) < Len) && (Line[Pos + 1] != '\n')){
			return(1);
		}
	}
	return(0);
}

static int gdwo__fix_index_import(int Index, int N){
	if (Index > 0){
		return(Index - 1);
	}
	if (Index == 0){
		return(0);
	}
	return(N + Index);
}

static void gdwo__init_material(gdwoMaterial* Material){
	Material->Name = 0;
	Material->AmbientTextureName = 0;
	Material->DiffuseTextureName = 0;
	Material->SpecularTextureName = 0;
	Material->SpecularHighlightTextureName = 0;
	Material->BumpTextureName = 0;
	Material->DisplacementTextureName = 0;
	Material->AlphaTextureName = 0;
	for (int i = 0; i < 3; i++){
		Material->Ambient[i] = 0.0f;
		Material->Diffuse[i] = 0.0f;
		Material->Specular[i] = 0.0f;
		Material->Transmittance[i] = 0.0f;
		Material->Emission[i] = 0.0f;
	}
	Material->Illum = 0;
	Material->Transparency = 1.0f;
	Material->Shininess = 1.0f;
	Material->RefractionCoefficient = 1.0f;
}

static gdwoMaterial* gdwo__material_add(
	gdwoMaterial* Prev,
	int MaterialsCount,
	gdwoMaterial* NewMaterial)
{
	gdwoMaterial* Dest;
	Dest = (gdwoMaterial*)realloc(Prev, sizeof(gdwoMaterial) * (MaterialsCount + 1));
	Dest[MaterialsCount] = (*NewMaterial);
	return(Dest);
}

static int gdwo__parse_mtl_file(
	gdwoMaterial** MaterialsOut,
	int* MaterialsOutCount,
	const char* FileName)
{
	gdwoMaterial Material;
	char LineBuffer[GDWO_MAX_LINE_LEN];
	FILE* fp;
	int NumberOfMaterials = 0;
	gdwoMaterial* Materials = 0;
	int HasPreviousMaterial = 0;

	if (MaterialsOut == 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}

	if (MaterialsOutCount == 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}

	fp = fopen(FileName, "r");
	if (!fp){
		return(GDWO_ERROR_FILE_OPERATION);
	}

	gdwo__init_material(&Material);

	while (fgets(LineBuffer, GDWO_MAX_LINE_LEN - 1, fp) != 0){
		const char* Token = LineBuffer;

		Token += strspn(Token, " \t");

		GDWO_ASSERT(Token);
		if (Token[0] == 0){
			continue;
		}

		if (Token[0] == '#'){
			continue;
		}

		if ((strncmp(Token, "newmtl", 6) == 0) && GDWO_IS_SPACE((Token[6]))){
			char NameBuffer[GDWO_MAX_LINE_LEN];

			if (HasPreviousMaterial){
				Materials = gdwo__material_add(Materials, NumberOfMaterials, &Material);
				NumberOfMaterials++;
			}
			else{
				HasPreviousMaterial = 1;
			}

			gdwo__init_material(&Material);

			Token += 7;
#ifdef _MSC_VER
			sscanf_s(Token, "%s", NameBuffer, (unsigned)_countof(NameBuffer));
#else
			sscanf(Token, "%s", NameBuffer);
#endif

			Material.Name = gdwo__strduplicate(NameBuffer);
			continue;
		}

		/*Ambient*/
		if (Token[0] == 'K' && Token[1] == 'a' && GDWO_IS_SPACE((Token[2]))){
			float R, G, B;
			Token += 2;
			gdwo__parse_float3(&R, &G, &B, &Token);
			Material.Ambient[0] = R;
			Material.Ambient[1] = G;
			Material.Ambient[2] = B;
			continue;
		}

		/*Diffuse*/
		if (Token[0] == 'K' && Token[1] == 'd' && GDWO_IS_SPACE((Token[2]))){
			float R, G, B;
			Token += 2;
			gdwo__parse_float3(&R, &G, &B, &Token);
			Material.Diffuse[0] = R;
			Material.Diffuse[1] = G;
			Material.Diffuse[2] = B;
			continue;
		}

		/*Specular*/
		if (Token[0] == 'K' && Token[1] == 's' && GDWO_IS_SPACE((Token[2]))){
			float R, G, B;
			Token += 2;
			gdwo__parse_float3(&R, &G, &B, &Token);
			Material.Specular[0] = R;
			Material.Specular[1] = G;
			Material.Specular[2] = B;
			continue;
		}

		/*Transmittance*/
		if (Token[0] == 'K' && Token[1] == 't' && GDWO_IS_SPACE((Token[2]))){
			float R, G, B;
			Token += 2;
			gdwo__parse_float3(&R, &G, &B, &Token);
			Material.Transmittance[0] = R;
			Material.Transmittance[1] = G;
			Material.Transmittance[2] = B;
			continue;
		}

		/*Emission*/
		if (Token[0] == 'K' && Token[1] == 'e' && GDWO_IS_SPACE((Token[2]))){
			float R, G, B;
			Token += 2;
			gdwo__parse_float3(&R, &G, &B, &Token);
			Material.Emission[0] = R;
			Material.Emission[1] = G;
			Material.Emission[2] = B;
			continue;
		}

		/*Index of refraction*/
		if (Token[0] == 'N' && Token[1] == 'i' && GDWO_IS_SPACE(Token[2])){
			Token += 2;
			Material.RefractionCoefficient = gdwo__parse_float(&Token);
			continue;
		}

		/*Shininess*/
		if (Token[0] == 'N' && Token[1] == 's' && GDWO_IS_SPACE(Token[2])){
			Token += 2;
			Material.Shininess = gdwo__parse_float(&Token);
			continue;
		}

		/*Illum*/
		if ((strncmp(Token, "illum", 5) == 0) && GDWO_IS_SPACE(Token[5])){
			Token += 6;
			Material.Illum = gdwo__parse_int(&Token);
			continue;
		}

		/*Transparency*/
		if ((Token[0] == 'd' && GDWO_IS_SPACE(Token[1]))){
			Token += 1;
			Material.Transparency = gdwo__parse_float(&Token);
			continue;
		}
		if (Token[0] == 'T' && Token[1] == 'r' && GDWO_IS_SPACE(Token[2])){
			Token += 2;
			Material.Transparency = 1.0f - gdwo__parse_float(&Token);
			continue;
		}

		/*Ambient texture*/
		if ((strncmp(Token, "map_Ka", 6) == 0) && GDWO_IS_SPACE(Token[6])){
			Token += 7;
			Material.AmbientTextureName = gdwo__strduplicate(Token);
			continue;
		}

		/*Diffuse texture*/
		if ((strncmp(Token, "map_Kd", 6) == 0) && GDWO_IS_SPACE(Token[6])){
			Token += 7;
			Material.DiffuseTextureName = gdwo__strduplicate(Token);
			continue;
		}

		/*Specular texture*/
		if ((strncmp(Token, "map_Ks", 6) == 0) && GDWO_IS_SPACE(Token[6])){
			Token += 7;
			Material.SpecularTextureName = gdwo__strduplicate(Token);
			continue;
		}

		/*Specular highlight*/
		if ((strncmp(Token, "map_Ns", 6) == 0) && GDWO_IS_SPACE(Token[6])){
			Token += 7;
			Material.SpecularHighlightTextureName = gdwo__strduplicate(Token);
			continue;
		}

		/*Bump texture*/
		if ((strncmp(Token, "map_bump", 8) == 0) && GDWO_IS_SPACE(Token[8])){
			Token += 9;
			Material.BumpTextureName = gdwo__strduplicate(Token);
			continue;
		}
		if ((strncmp(Token, "bump", 5) == 0) && GDWO_IS_SPACE(Token[5])){
			Token += 6;
			Material.BumpTextureName = gdwo__strduplicate(Token);
			continue;
		}

		/*Alpha texture*/
		if ((strncmp(Token, "map_d", 5) == 0) && GDWO_IS_SPACE(Token[5])){
			Token += 6;
			Material.AlphaTextureName = gdwo__strduplicate(Token);
			continue;
		}

		/*Displacement texture*/
		if ((strncmp(Token, "disp", 5) == 0) && GDWO_IS_SPACE(Token[5])){
			Token += 6;
			Material.DisplacementTextureName = gdwo__strduplicate(Token);
			continue;
		}
	}

	if (Material.Name){
		Materials = gdwo__material_add(Materials, NumberOfMaterials, &Material);
		NumberOfMaterials++;
	}

	(*MaterialsOutCount) = NumberOfMaterials;
	(*MaterialsOut) = Materials;
}

typedef struct gdwoLineInfo{
	int Pos;
	int Len;
} gdwoLineInfo;

int gd_parse_obj(
	gdwoMeshAttrib* Attrib,
	gdwoShape** Shapes, int* NumberOfShapes,
	gdwoMaterial** MaterialsOut, int* NumberOfMaterialsOut,
	const char* Buffer, int BufferLen,
	unsigned int Flags)
{
	gdwoLineInfo* LineInfos = 0;
	gdwoParseResult* ParseResults = 0;
	int NumLines = 0;

	int VertexCount = 0;
	int NormalCount = 0;
	int TexCoordCount = 0;
	int FaceIndicesCount = 0;
	int NumberOfFaceIndicesCount = 0;

	int MtlLibLine = -1;

	gdwoMaterial* Materials = 0;
	int CountMaterials = 0;

	if (BufferLen < 1){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}
	if (Attrib == 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}
	if (Shapes == 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}
	if (NumberOfShapes == 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}
	if (Buffer == 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}
	if (MaterialsOut == 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}
	if (NumberOfMaterialsOut = 0){
		return(GDWO_ERROR_INVALID_PARAMETER);
	}

	gd_attrib_init(Attrib);

	/*Find and create line data*/
	{
		int End = BufferLen - 1;
		int PrevPos = 0;
		int LineNo = 0;

		/*Count of lines*/
		for (int i = 0; i < End; i++){
			if (gdwo__is_line_end(Buffer, i, End)){
				NumLines++;
			}
		}
		
		if (NumLines == 0){
			return(GDWO_ERROR_EMPTY);
		}

		LineInfos = (gdwoLineInfo*)malloc(sizeof(gdwoLineInfo) * NumLines);

		for (int i = 0; i < End; i++){
			if (gdwo__is_line_end(Buffer, i, End)){
				LineInfos[LineNo].Pos = PrevPos;
				LineInfos[LineNo].Len = i - PrevPos;
				PrevPos = i + 1;
				LineNo++;
			}
		}
	}

	ParseResults = (gdwoParseResult*)malloc(sizeof(gdwoParseResult) * NumLines);

	/*Now we need to parse each line*/
	{
		for (int i = 0; i < NumLines; i++){
			int LineParsed = gdwo__parse_line(
				&ParseResults[i],
				&Buffer[LineInfos[i].Pos],
				LineInfos[i].Len, Flags & GDWO_FLAG_TRIANGULATE);

			if (LineParsed){
				switch (ParseResults[i].Type){
					case TokenType_V:{
						VertexCount++;
					}break;
					case TokenType_VN:{
						NormalCount++;
					}break;
					case TokenType_VT:{
						TexCoordCount++;
					}break;
					case TokenType_F:{
						FaceIndicesCount += ParseResults[i].FaceIndicesCount;
						NumberOfFaceIndicesCount += ParseResults[i].NumberOfIndicesCount;
					}break;
					case TokenType_MTLLIB:{
						MtlLibLine = i;
					}break;
				}
			}
		}
	}

	if (LineInfos){
		free(LineInfos);
	}

	/*Load material*/
	if (MtlLibLine >= 0 && ParseResults[MtlLibLine].MTLLibName &&
		ParseResults[MtlLibLine].MTLLibNameLen > 0)
	{
		char* FileName = gdwo__strnduplicate(
			ParseResults[MtlLibLine].MTLLibName,
			ParseResults[MtlLibLine].MTLLibNameLen);

		int ParseMTLRes = gdwo__parse_mtl_file(&Materials, &CountMaterials, FileName);

		if (ParseMTLRes != GDWO_ERROR_SUCCESS){
			fprintf(stderr, "GDWO: Failed to parse .mtl file: %s\n", FileName);
		}

		free(FileName);
	}

	/*Construct attributes*/
	{
		int TempVertexCount = 0;
		int TempNormalCount = 0;
		int TempTexCoordCount = 0;
		int TempFaceIndicesCount = 0;
		int TempNumberOfFaceIndicesCount = 0;

		int MaterialId = -1;

		Attrib->Vertices = (float*)malloc(sizeof(float) * VertexCount * 3);
		Attrib->NumberOfVertices = (unsigned int)VertexCount;
		Attrib->Normals = (float*)malloc(sizeof(float) * NormalCount * 3);
		Attrib->NumberOfNormals = (unsigned int)NormalCount;
		Attrib->TexCoords = (float*)malloc(sizeof(float) * TexCoordCount * 2);
		Attrib->NumberOfTexCoords = (unsigned int)TexCoordCount;
		
		Attrib->Faces = (gdwoVertexIndices*)malloc(sizeof(gdwoVertexIndices) * FaceIndicesCount);
		Attrib->NumberOfFaces = (unsigned int)FaceIndicesCount;

		Attrib->FaceNumVertices = (int*)malloc(sizeof(int) * NumberOfFaceIndicesCount);
		Attrib->MaterialsIDs = (int*)malloc(sizeof(int) * NumberOfFaceIndicesCount);
		Attrib->NumberOfFaceVertices = (unsigned int)NumberOfFaceIndicesCount;

		for (int i = 0; i < NumLines; i++){
			switch (ParseResults[i].Type){
				case TokenType_Empty:{
					continue;
				}break;
				case TokenType_USEMTL:{
					
				}break;
				case TokenType_V:{
					Attrib->Vertices[3 * TempVertexCount + 0] = ParseResults[i].VertX;
					Attrib->Vertices[3 * TempVertexCount + 1] = ParseResults[i].VertY;
					Attrib->Vertices[3 * TempVertexCount + 2] = ParseResults[i].VertZ;
					TempVertexCount++;
				}break;
				case TokenType_VN:{
					Attrib->Normals[3 * TempNormalCount + 0] = ParseResults[i].NormX;
					Attrib->Normals[3 * TempNormalCount + 1] = ParseResults[i].NormY;
					Attrib->Normals[3 * TempNormalCount + 2] = ParseResults[i].NormZ;
				}break;
				case TokenType_VT:{
					Attrib->TexCoords[2 * TempTexCoordCount + 0] = ParseResults[i].TexCoordX;
					Attrib->TexCoords[2 * TempTexCoordCount + 1] = ParseResults[i].TexCoordY;
				}break;
				case TokenType_F:{
					for (int j = 0; j < ParseResults[i].FaceIndicesCount; j++){
						gdwoVertexIndices VertInd = ParseResults[i].FaceIndices[j];
						int VertexIndex = gdwo__fix_index_import(VertInd.VertexIndex, TempVertexCount);
						int NormalIndex = gdwo__fix_index_import(VertInd.NormalIndex, TempNormalCount);
						int TexCoordIndex = gdwo__fix_index_import(VertInd.TexCoordIndex, TempTexCoordCount);

						Attrib->Faces[TempFaceIndicesCount + j].VertexIndex = VertexIndex;
						Attrib->Faces[TempFaceIndicesCount + j].NormalIndex = NormalIndex;
						Attrib->Faces[TempFaceIndicesCount + j].TexCoordIndex = TexCoordIndex;
					}

					for (int j = 0; j < ParseResults[i].NumberOfIndicesCount; j++){
						Attrib->MaterialsIDs[TempNumberOfFaceIndicesCount + j] = MaterialId;
						Attrib->FaceNumVertices[TempNumberOfFaceIndicesCount + j] = ParseResults[i].NumberOfIndices[j];
					}

					TempFaceIndicesCount += ParseResults[i].FaceIndicesCount;
					TempNumberOfFaceIndicesCount += ParseResults[i].NumberOfIndicesCount;
				}break;
			}
		}
	}

	/*Construct shape information*/
	{
		unsigned int TempFaceCount = 0;
		
		int N = 0;
		int ShapeIndex = 0;

		const char* ShapeName = 0;
		unsigned int ShapeNameLen = 0;
		const char* PrevShapeName = 0;
		unsigned int PrevShapeNameLen = 0;
		unsigned int PrevShapeFaceOffset = 0;
		unsigned int PrevShapeLength = 0;
		unsigned int PrevFaceOffset = 0;
		gdwoShape PrevShape = { 0, 0, 0 };

		for (int i = 0; i < NumLines; i++){
			if (ParseResults[i].Type == TokenType_O || ParseResults[i].Type == TokenType_G){
				N++;
			}
		}

		(*Shapes) = (gdwoShape*)malloc(sizeof(gdwoShape) * (N + 1));

		for (int i = 0; i < NumLines; i++){
			if (ParseResults[i].Type == TokenType_O || ParseResults[i].Type == TokenType_G){
				if (ParseResults[i].Type == TokenType_O){
					ShapeName = ParseResults[i].ObjectName;
					ShapeNameLen = ParseResults[i].ObjectNameLen;
				}
				else{
					ShapeName = ParseResults[i].GroupName;
					ShapeNameLen = ParseResults[i].GroupNameLen;
				}

				if (TempFaceCount == 0){
					PrevShapeName = ShapeName;
					PrevShapeNameLen = ShapeNameLen;
					PrevShapeFaceOffset = TempFaceCount;
					PrevFaceOffset = TempFaceCount;
				}
				else{
					if (ShapeIndex == 0){
						(*Shapes)[ShapeIndex].Name = gdwo__strnduplicate(PrevShapeName, PrevShapeNameLen);
						(*Shapes)[ShapeIndex].FaceOffset = PrevShape.FaceOffset;
						(*Shapes)[ShapeIndex].Length = TempFaceCount - PrevFaceOffset;
						ShapeIndex++;
						PrevShapeLength = TempFaceCount - PrevFaceOffset;
						PrevFaceOffset = TempFaceCount;
					}
					else{
						if ((TempFaceCount - PrevFaceOffset) > 0){
							(*Shapes)[ShapeIndex].Name = gdwo__strnduplicate(PrevShapeName, PrevShapeNameLen);
							(*Shapes)[ShapeIndex].FaceOffset = PrevFaceOffset;
							(*Shapes)[ShapeIndex].Length = TempFaceCount - PrevFaceOffset;
							ShapeIndex++;
							PrevShapeLength = TempFaceCount - PrevFaceOffset;
							PrevFaceOffset = TempFaceCount;
						}
					}

					PrevShapeName = ShapeName;
					PrevShapeNameLen = ShapeNameLen;
					PrevShapeFaceOffset = TempFaceCount;
					PrevShapeLength = 0;
				}
			}

			if (ParseResults[i].Type == TokenType_F){
				TempFaceCount++;
			}
		}

		if ((TempFaceCount - PrevFaceOffset) > 0){
			int Length = TempFaceCount - PrevShapeFaceOffset;
			if (Length > 0){
				(*Shapes)[ShapeIndex].Name = gdwo__strnduplicate(PrevShapeName, PrevShapeNameLen);
				(*Shapes)[ShapeIndex].FaceOffset = PrevFaceOffset;
				(*Shapes)[ShapeIndex].Length = TempFaceCount - PrevFaceOffset;
				ShapeIndex++;
			}
		}
		else{

		}

		(*NumberOfShapes) = ShapeIndex;
	}

	if (ParseResults){
		free(ParseResults);
	}

	(*MaterialsOut) = Materials;
	(*NumberOfMaterialsOut) = CountMaterials;

	return(GDWO_ERROR_SUCCESS);
}

void gd_attrib_init(gdwoMeshAttrib* Attrib){
	Attrib->Vertices = 0;
	Attrib->NumberOfVertices = 0;
	Attrib->Normals = 0;
	Attrib->NumberOfNormals = 0;
	Attrib->TexCoords = 0;
	Attrib->NumberOfTexCoords = 0;
	Attrib->Faces = 0;
	Attrib->NumberOfFaces = 0;
	Attrib->FaceNumVertices = 0;
	Attrib->NumberOfFaceVertices = 0;
	Attrib->MaterialsIDs = 0;
}

void gd_attrib_free(gdwoMeshAttrib* Attrib){
	if (Attrib->Vertices){
		free(Attrib->Vertices);
	}
	if (Attrib->Normals){
		free(Attrib->Normals);
	}
	if (Attrib->TexCoords){
		free(Attrib->TexCoords);
	}
	if (Attrib->Faces){
		free(Attrib->Faces);
	}
	if (Attrib->FaceNumVertices){
		free(Attrib->FaceNumVertices);
	}
	if (Attrib->MaterialsIDs){
		free(Attrib->MaterialsIDs);
	}
}

void gd_shapes_free(gdwoShape* Shapes, int NumberOfShapes){
	if (Shapes == 0){
		return;
	}

	for (int i = 0; i < NumberOfShapes; i++){
		if (Shapes[i].Name){
			free(Shapes[i].Name);
		}
	}

	free(Shapes);
}

void gd_materials_free(gdwoMaterial* Materials, int NumberOfMaterials){
	if (Materials == 0){
		return;
	}

	for (int i = 0; i < NumberOfMaterials; i++){
		if (Materials[i].Name){
			free(Materials[i].Name);
		}
		if (Materials[i].AmbientTextureName){
			free(Materials[i].AmbientTextureName);
		}
		if (Materials[i].DiffuseTextureName){
			free(Materials[i].DiffuseTextureName);
		}
		if (Materials[i].SpecularTextureName){
			free(Materials[i].SpecularTextureName);
		}
		if (Materials[i].SpecularHighlightTextureName){
			free(Materials[i].SpecularHighlightTextureName);
		}
		if (Materials[i].BumpTextureName){
			free(Materials[i].BumpTextureName);
		}
		if (Materials[i].DiffuseTextureName){
			free(Materials[i].DisplacementTextureName);
		}
		if (Materials[i].AlphaTextureName){
			free(Materials[i].AlphaTextureName);
		}
	}

	free(Materials);
}

#endif