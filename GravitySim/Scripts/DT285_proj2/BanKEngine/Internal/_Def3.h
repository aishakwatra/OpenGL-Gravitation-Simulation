#include "_Def2.h"




namespace Input {
	SDL_Event Event;

	namespace Key {
		const Uint8* KeyboardState;

		struct EachKey;
		vector <EachKey*> Events;
		class EachKey {
			int _MyCode;
		public:
			EachKey(int getC) : _MyCode(getC) { Events.push_back(this); }
			bool Pressed = false;
			bool OnPressed = false;
			bool OnReleased = false;
			void Update(const Uint8* KeyboardState) {
				if (KeyboardState[_MyCode]) {
					if (Pressed) {
						OnPressed = false;
					}
					else {
						OnPressed = true; //cout << "\n key is ONpressed!";
					}
					Pressed = true;		OnReleased = false;
				}
				else {

					if (Pressed) {
						OnReleased = true; //cout << "\n key is ONreleased!";
					}
					else {
						OnReleased = false;
					}
					Pressed = false;	OnPressed = false;
				}
			}
		};
		void Update() {
			KeyboardState = SDL_GetKeyboardState(NULL); //numkeys????????????????????
			for (EachKey* E : Events) { E->Update(KeyboardState); }
		}
		EachKey A(SDL_SCANCODE_A), B(SDL_SCANCODE_B), C(SDL_SCANCODE_C), D(SDL_SCANCODE_D), E(SDL_SCANCODE_E), F(SDL_SCANCODE_F), G(SDL_SCANCODE_G), H(SDL_SCANCODE_H), I(SDL_SCANCODE_I), J(SDL_SCANCODE_J), K(SDL_SCANCODE_K), L(SDL_SCANCODE_L), M(SDL_SCANCODE_M), N(SDL_SCANCODE_N), O(SDL_SCANCODE_O), P(SDL_SCANCODE_P), Q(SDL_SCANCODE_Q), R(SDL_SCANCODE_R), S(SDL_SCANCODE_S), T(SDL_SCANCODE_T), U(SDL_SCANCODE_U), V(SDL_SCANCODE_V), W(SDL_SCANCODE_W), X(SDL_SCANCODE_X), Y(SDL_SCANCODE_Y), Z(SDL_SCANCODE_Z);
		EachKey _0(SDL_SCANCODE_0), _1(SDL_SCANCODE_1), _2(SDL_SCANCODE_2), _3(SDL_SCANCODE_3), _4(SDL_SCANCODE_4), _5(SDL_SCANCODE_5), _6(SDL_SCANCODE_6), _7(SDL_SCANCODE_7), _8(SDL_SCANCODE_8), _9(SDL_SCANCODE_9);
		EachKey ExclamationMark(SDL_SCANCODE_GRAVE), At(SDL_SCANCODE_2), Hash(SDL_SCANCODE_3), Dollar(SDL_SCANCODE_4), Percent(SDL_SCANCODE_5), Caret(SDL_SCANCODE_6), Ampersand(SDL_SCANCODE_7), Asterisk(SDL_SCANCODE_8), OpenParen(SDL_SCANCODE_9), CloseParen(SDL_SCANCODE_0), Minus(SDL_SCANCODE_MINUS), Underscore(SDL_SCANCODE_MINUS), Plus(SDL_SCANCODE_EQUALS), Equals(SDL_SCANCODE_EQUALS), OpenBracket(SDL_SCANCODE_LEFTBRACKET), CloseBracket(SDL_SCANCODE_RIGHTBRACKET), CurlyBraceOpen(SDL_SCANCODE_LEFTBRACKET), CurlyBraceClose(SDL_SCANCODE_RIGHTBRACKET), Semicolon(SDL_SCANCODE_SEMICOLON), Colon(SDL_SCANCODE_SEMICOLON), Quote(SDL_SCANCODE_APOSTROPHE), DoubleQuote(SDL_SCANCODE_APOSTROPHE), Comma(SDL_SCANCODE_COMMA), LessThan(SDL_SCANCODE_COMMA), Period(SDL_SCANCODE_PERIOD), GreaterThan(SDL_SCANCODE_PERIOD), Slash(SDL_SCANCODE_SLASH), QuestionMark(SDL_SCANCODE_SLASH), Backslash(SDL_SCANCODE_BACKSLASH), Tilde(SDL_SCANCODE_GRAVE), GraveAccent(SDL_SCANCODE_GRAVE);
		EachKey Space(SDL_SCANCODE_SPACE);

	}

	namespace Controller {
		int numJoysticks = SDL_NumJoysticks();

		struct EachButton;
		vector <EachButton*> Events;
		class EachButton {
			int _MyCode;
		public:
			EachButton(int getC) : _MyCode(getC) { Events.push_back(this); }
			bool Pressed = false;
			bool OnPressed = false;
			bool OnReleased = false;
			void Update(const Uint8* KeyboardState) {
				if (KeyboardState[_MyCode]) {
					if (Pressed) {
						OnPressed = false;
					}
					else {
						OnPressed = true; cout << "\n key is ONpressed!";
					}
					Pressed = true;		OnReleased = false;
				}
				else {

					if (Pressed) {
						OnReleased = true; cout << "\n key is ONreleased!";
					}
					else {
						OnReleased = false;
					}
					Pressed = false;	OnPressed = false;
				}
			}
		};
		void Update() {
			//KeyboardState = SDL_GetKeyboardState(NULL); //numkeys????????????????????
			//for (EachButton* E : Events) {  E->Update(KeyboardState); } 
		}
	}

	void Update() {
		SDL_PollEvent(&Event);
		Key::Update();
	}
}

