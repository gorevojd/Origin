#ifndef GDA_SORTING

#ifndef GDA_SORTING_DEFINITIONS

#define GDA_SORTING_QUICK
#define GDA_SORTING_INSERT
#define GDA_SORTING_BUBBLE

void gda_SwapInt(int* A, int* B);
void gda_SwapFloat(float* A, float* B);
void gda_SwapDouble(double* A, double* B);

void gda_QuickSortInt(int* Array, int Begin, int End);
void gda_QuickSortFloat(float* Array, int Begin, int End);
void gda_QuickSortDouble(double* Array, int Begin, int End);
void gda_QuickSortIntDesc(int* Array, int Begin, int End);
void gda_QuickSortFloatDesc(float* Array, int Begin, int End);
void gda_QuickSortDoubleDesc(double* Array, int Begin, int End);

void gda_InsertSortInt(int* Array, int Size);
void gda_InsertSortFloat(float* Array, int Size);
void gda_InsertSortDouble(double* Array, int Size);
void gda_InsertSortIntDesc(int* Array, int Size);
void gda_InsertSortFloatDesc(float* Array, int Size);
void gda_InsertSortDoubleDesc(double* Array, int Size);

void gda_BubbleSortInt(int* Array, int Size);
void gda_BubbleSortFloat(float* Array, int Size);
void gda_BubbleSortDouble(double* Array, int Size);
void gda_BubbleSortIntDesc(int* Array, int Size);
void gda_BubbleSortFloatDesc(float* Array, int Size);
void gda_BubbleSortDoubleDesc(double* Array, int Size);

#define GDA_SORTING_DEFINITIONS
#endif

#ifndef GDA_SORTING_IMPLEMENTATION
#define GDA_SORTING_IMPLEMENTATION
#endif

#ifdef GDA_SORTING_IMPLEMENTATION


void gda_SwapInt(int* A, int* B){
	int Temp = *A;
	*A = *B;
	*B = Temp;
}

void gda_SwapFloat(float* A, float* B){
	float Temp = *A;
	*A = *B;
	*B = Temp;
}

void gda_SwapDouble(double* A, double* B){
	double Temp = *A;
	*A = *B;
	*B = Temp;
}

#ifdef GDA_SORTING_QUICK
void gda_QuickSortInt(int* Array, int Begin, int End){
	int Left = Begin;
	int Right = End;

	int Mediana = Array[(End + Begin) / 2];

	while (Left <= Right){
		while (Array[Left] < Mediana){
			Left++;
		}
		while (Array[Right] > Mediana){
			Right--;
		}
		if (Left <= Right){
			gda_SwapInt(&Array[Left], &Array[Right]);

			Left++;
			Right--;
		}
	}

	if (Right > Begin){
		gda_QuickSortInt(Array, Begin, Right);
	}
	if (Left < End){
		gda_QuickSortInt(Array, Left, End);
	}
}

void gda_QuickSortFloat(float* Array, int Begin, int End){
	int Left = Begin;
	int Right = End;

	float Mediana = Array[(End + Begin) / 2];

	while (Left <= Right){
		while (Array[Left] < Mediana){
			Left++;
		}
		while (Array[Right] > Mediana){
			Right--;
		}
		if (Left <= Right){
			gda_SwapFloat(&Array[Left], &Array[Right]);

			Left++;
			Right--;
		}
	}

	if (Right > Begin){
		gda_QuickSortFloat(Array, Begin, Right);
	}
	if (Left < End){
		gda_QuickSortFloat(Array, Left, End);
	}
}

void gda_QuickSortDouble(double* Array, int Begin, int End){
	int Left = Begin;
	int Right = End;

	double Mediana = Array[(End + Begin) / 2];

	while (Left <= Right){
		while (Array[Left] < Mediana){
			Left++;
		}
		while (Array[Right] > Mediana){
			Right--;
		}
		if (Left <= Right){
			gda_SwapDouble(&Array[Left], &Array[Right]);

			Left++;
			Right--;
		}
	}

	if (Right > Begin){
		gda_QuickSortDouble(Array, Begin, Right);
	}
	if (Left < End){
		gda_QuickSortDouble(Array, Left, End);
	}
}
#endif

#ifdef GDA_SORTING_INSERT
void gda_InsertSortInt(int* Array, int Size){
	for (int j = 0; j < Size - 1; j++){
		for (int i = Size - 1; i >= 0; i--){
			if (Array[i] < Array[i - 1]){
				gda_SwapInt(&Array[i], &Array[i - 1]);
			}
			else{
				break;
			}
		}
	}
}

void gda_InsertSortIntDesc(int* Array, int Size){
	for (int j = 0; j < Size - 1; j++){
		for (int i = Size - 1; i >= 0; i--){
			if (Array[i] > Array[i - 1]){
				gda_SwapInt(&Array[i], &Array[i - 1]);
			}
			else{
				break;
			}
		}
	}
}


void gda_InsertSortFloat(float* Array, int Size){
	for (int j = 0; j < Size - 1; j++){
		for (int i = Size - 1; i >= 0; i--){
			if (Array[i] < Array[i - 1]){
				gda_SwapFloat(&Array[i], &Array[i - 1]);
			}
			else{
				break;
			}
		}
	}
}


void gda_InsertSortFloatDesc(float* Array, int Size){
	for (int j = 0; j < Size - 1; j++){
		for (int i = Size - 1; i >= 0; i--){
			if (Array[i] > Array[i - 1]){
				gda_SwapFloat(&Array[i], &Array[i - 1]);
			}
			else{
				break;
			}
		}
	}
}


void gda_InsertSortDouble(double* Array, int Size){
	for (int j = 0; j < Size - 1; j++){
		for (int i = Size - 1; i >= 0; i--){
			if (Array[i] < Array[i - 1]){
				gda_SwapDouble(&Array[i], &Array[i - 1]);
			}
			else{
				break;
			}
		}
	}
}

void gda_InsertSortDoubleDesc(double* Array, int Size){
	for (int j = 0; j < Size - 1; j++){
		for (int i = Size - 1; i >= 0; i--){
			if (Array[i] > Array[i - 1]){
				gda_SwapDouble(&Array[i], &Array[i - 1]);
			}
			else{
				break;
			}
		}
	}
}
#endif

#ifdef GDA_SORTING_BUBBLE
//TODO(Dima): Test those
void gda_BubbleSortInt(int* Array, int Size){
	for(int i = 0; i < Size - 1; i++){
		for(int j = 0; j < Size - 1 - i; j++){
			if(Array[j] > Array[j + 1]){
				gda_SwapInt(&Array[j], &Array[j + 1]);
			}
		}
	}
}

void gda_BubbleSortIntDesc(int* Array, int Size){
	for (int i = 0; i < Size - 1; i++){
		for (int j = 0; j < Size - 1 - i; j++){
			if (Array[j] < Array[j + 1]){
				gda_SwapInt(&Array[j], &Array[j + 1]);
			}
		}
	}
}

void gda_BubbleSortFloat(float* Array, int Size){
	for(int i = 0; i < Size - 1; i++){
		for(int j = 0; j < Size - 1 - i; j++){
			if(Array[j] > Array[j + 1]){
				gda_SwapFloat(&Array[j], &Array[j + 1]);
			}
		}	
	}
}

void gda_BubbleSortFloatDesc(float* Array, int Size){
	for (int i = 0; i < Size - 1; i++){
		for (int j = 0; j < Size - 1 - i; j++){
			if (Array[j] < Array[j + 1]){
				gda_SwapFloat(&Array[j], &Array[j + 1]);
			}
		}
	}
}

void gda_BubbleSortDouble(double* Array, int Size){
	for(int i = 0; i < Size - 1; i++){
		for(int j = 0; j < Size - 1 - i; j++){
			if(Array[j] > Array[j + 1]){
				gda_SwapDouble(&Array[j], &Array[j + 1]);
			}
		}	
	}	
}

void gda_BubbleSortDoubleDesc(double* Array, int Size){
	for (int i = 0; i < Size - 1; i++){
		for (int j = 0; j < Size - 1 - i; j++){
			if (Array[j] < Array[j + 1]){
				gda_SwapDouble(&Array[j], &Array[j + 1]);
			}
		}
	}
}
#endif

#endif

#define GDA_SORTING
#endif