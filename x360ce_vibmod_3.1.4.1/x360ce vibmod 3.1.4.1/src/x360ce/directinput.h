#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

HRESULT InitDirectInput(HWND, INT,INT);
BOOL ButtonPressed(DWORD, INT);
INT POVState(DWORD, INT);

VOID Initialize(DWORD);
HRESULT Update(HWND, INT);
HRESULT SetDeviceForces(INT idx, WORD force, INT type);
HRESULT PrepareForce(INT idx, INT effidx);

extern LPDIRECTINPUT8 g_pDI;

struct DIGAMEPAD {
	TCHAR ident[9];
	LPDIRECTINPUTDEVICE8 g_pGamepad;
	INT id;
	TCHAR name[MAX_PATH];
	DWORD vid;
	DWORD pid;
	DIJOYSTATE2 state;
	LPDIRECTINPUTEFFECT g_pEffect[2];
	DWORD g_dwNumForceFeedbackAxis;
	INT axislinear[4];
	BOOL native;
	BOOL swapmotor;
	DWORD tdeadzone;
	BOOL useforce;
	DWORD forcepercent;
	BOOL povxrev;
	BOOL povyrev;
	INT gamepadtype;
	BOOL axistodpad;
	INT axistodpaddeadzone;
	INT axistodpadoffset;
	DIGAMEPAD()
	{
		_tcscpy_s(ident,_T("[x360ce]"));
		id = -1;
		for (INT i = 0; i < 2; ++i) g_pEffect[i] = NULL;
		g_dwNumForceFeedbackAxis = NULL;
		for (INT i = 0; i < 4; ++i) axislinear[i] = NULL;
		native = 0;
		swapmotor = 0;
		tdeadzone  = 0;
		useforce = 0;
		forcepercent = 100;
		povxrev = 0;
		povyrev = 0;
		gamepadtype = 1;

	}
};

// externs

extern struct DIGAMEPAD Gamepad[4];
extern VOID Initialize(DWORD idx);
extern HRESULT UpdateState( INT );
extern HRESULT InitDirectInput( HWND hook, INT idx );
extern VOID FreeDirectInput( INT idx );
extern WORD ConfigPadCount();
extern WORD EnumPadCount();
extern HRESULT Enumerate(DWORD idx);