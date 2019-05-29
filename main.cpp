/**
 * @author [Hasan Hüseyin CİHANGİR]
 * @email [hhuseyincihangir@gmail.com]
 * @create date 2019-05-29 11:52:32
 * @modify date 2019-05-29 11:52:49
 * @desc [description]
 */


#include <iostream>
#include <string>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

using namespace std;

short kullaniciId = 1;
short roundSayisi = 0;
bool oyunBitti = false;

short skorX = 0, skorO = 0;

short haritaGenisligi = 600;
short haritaYuksekligi = 600;

short hucreBoyutu = 200;

short karekterBoyutu = 32;

short durum[3][3] = {
	{0,0,0},
	{0,0,0},
	{0,0,0},
};
unsigned short harita[3][3][2]{
	{{0,16},{200,16},{400,16}},
	{{0,216},{200,216},{400,216}},
	{{0,416},{200,416},{400,416}},
};


// UI Değişkenleri
sf::Text roundSayisiText;
sf::Text oyuncuXMetin;
sf::Text oyuncuOMetin;

sf::Vector2f mousePozisyon;
bool fareKontrol(sf::Vector2f farePozisyon, short kulaniciId);
void elSifirla(short durumMatrisi[3][3]);
bool bitisKontrol(short durumMatrisi[3][3], short kullaniciId);
bool berabereKontrol();

sf::Font font;
sf::Font fontYukle(string fontYolu);

void oyunEkrani(sf::RenderWindow *window);

sf::Text temizle(float x, float y,string metin);
sf::Text xCiz(float x, float y);
sf::Text oCiz(float x, float y);

int main()
{
	sf::RenderWindow window(sf::VideoMode(haritaGenisligi,haritaYuksekligi+50),"Tic-Tac-Toe",sf::Style::Close);//sf::Style::Fullscreen
	window.setFramerateLimit(30);

	font = fontYukle("neuropol.ttf");
	
	while(window.isOpen())
	{
		sf::Event event;
		
		mousePozisyon = sf::Vector2f();
		
		if (oyunBitti){
			// Yeniden durum ve skor sıfırlanacak
		}
		window.clear(sf::Color(0,0,0));
			oyunEkrani(&window);
			// Girdi kontrol
			while(window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed){
					window.close();				
				}
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
					window.close();
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					mousePozisyon = sf::Vector2f(event.touch.x,event.touch.y);
					if (fareKontrol(mousePozisyon, kullaniciId)){
						cout<<kullaniciId<<endl;
						if (kullaniciId == 1){
							if (bitisKontrol(durum, kullaniciId)){
								skorX += 1;
								if ( !(skorX >=5)){
									elSifirla(durum);
									roundSayisi++;
								}else{
									window.close();
								}
							}
							else if(berabereKontrol()){
								elSifirla(durum);
								roundSayisi++;
							}
							kullaniciId = 2;
						}
						else if (kullaniciId == 2){
							if (bitisKontrol(durum, kullaniciId)){
								skorO += 1;
								if ( !(skorO >= 5)){
									elSifirla(durum);
									roundSayisi++;
								}else{
									window.close();
								}
							}
							else if(berabereKontrol()){
								elSifirla(durum);
								roundSayisi++;
							}	
							kullaniciId = 1;
						}
					}					
				}
			}
			// ./Girdi kontrol
			
		window.display();
	}
    return EXIT_SUCCESS;
}
void oyunEkrani(sf::RenderWindow * window){
	// Arkaplan
	short sayac = 0;
	for (short x=0;x<3;x++){
		for(short y=0;y<3;y++){
			sf::RectangleShape kare;
				kare.setSize(sf::Vector2f(hucreBoyutu,hucreBoyutu));
				kare.setPosition(x*hucreBoyutu,y*hucreBoyutu+50);
				kare.setFillColor(sf::Color::Black);
				kare.setOutlineThickness(2.0f);
				kare.setOutlineColor(sf::Color::White);					
			window->draw(kare);
			sayac++;			
		}
	}
	// ./Arkaplan
	// UI
	roundSayisiText.setFont(font);
	roundSayisiText.setPosition(haritaGenisligi/2-karekterBoyutu/2,5);
	roundSayisiText.setCharacterSize(karekterBoyutu);
	roundSayisiText.setFillColor(sf::Color::White);
	roundSayisiText.setString("-"+to_string(roundSayisi)+"-");
	
	oyuncuXMetin.setFont(font);
	oyuncuXMetin.setPosition(10,5);
	oyuncuXMetin.setCharacterSize(karekterBoyutu);
	oyuncuXMetin.setFillColor(sf::Color::White);
	
	oyuncuOMetin.setFont(font);
	oyuncuOMetin.setPosition(haritaGenisligi-200,5);
	oyuncuOMetin.setCharacterSize(karekterBoyutu);
	oyuncuOMetin.setFillColor(sf::Color::White);
		
	if (kullaniciId == 1){
		oyuncuXMetin.setString("-> X = "+to_string(skorX));
	}else{
		oyuncuXMetin.setString("   X = "+to_string(skorX));
	}
	if (kullaniciId == 2){
		oyuncuOMetin.setString("-> O = "+to_string(skorO));
	}else{
		oyuncuOMetin.setString("   O = "+to_string(skorO));		
	}
	
	window->draw(roundSayisiText);
	window->draw(oyuncuXMetin);
	window->draw(oyuncuOMetin);
	
	// ./UI
	
	// Oyun Çizdirme		
	for (short y=0;y<3;y++){
		for (short x=0;x<3;x++){
			if (durum[x][y]==1){
				window->draw(xCiz(harita[x][y][0],harita[x][y][1]));				
			}
			else if(durum[x][y]==2){
				window->draw(oCiz(harita[x][y][0],harita[x][y][1]));
			}
			else{
				window->draw(temizle(harita[x][y][0],harita[x][y][1]," "));
			}
		}
	}
	// ./Oyun Çizdirme
}

bool fareKontrol(sf::Vector2f farePozisyon, short kulaniciId){
	if      ( (farePozisyon.x < 200 and farePozisyon.x > 0) and (farePozisyon.y <= 250 and farePozisyon.y > 50) ){//x=0, y=0. Hücre
		if (durum[0][0] == 0){
			durum[0][0] = kulaniciId;
			return true;			
		}
	}
	else if ( (farePozisyon.x < 400 and farePozisyon.x > 200) and (farePozisyon.y <= 250 and farePozisyon.y > 50) ){
		if (durum[0][1] == 0){
			durum[0][1] = kulaniciId;
			return true;
		}
	}
	else if ( (farePozisyon.x < 600 and farePozisyon.x > 400) and (farePozisyon.y <= 250 and farePozisyon.y > 50) ){
		if (durum[0][2] == 0){
			durum[0][2] = kulaniciId;
			return true;
		}
	}
	else if ( (farePozisyon.x < 200 and farePozisyon.x > 0) and (farePozisyon.y <= 450 and farePozisyon.y > 250) ){
		if (durum[1][0] == 0){
			durum[1][0] = kulaniciId;		
			return true;
		}
	}
	else if ( (farePozisyon.x < 400 and farePozisyon.x > 200) and (farePozisyon.y <= 450 and farePozisyon.y > 250) ){
		if (durum[1][1] == 0){
			durum[1][1] = kulaniciId;
			return true;
		}
	}
	else if ( (farePozisyon.x < 600 and farePozisyon.x > 400) and (farePozisyon.y <= 450 and farePozisyon.y > 250) ){
		if (durum[1][2] == 0){
			durum[1][2] = kulaniciId;
			return true;
		}
	}
	else if ( (farePozisyon.x < 200 and farePozisyon.x > 0) and (farePozisyon.y <= 650 and farePozisyon.y > 450) ){
		if (durum[2][0] == 0){
			durum[2][0] = kulaniciId;
			return true;
		}
	}
	else if ( (farePozisyon.x < 400 and farePozisyon.x > 200) and (farePozisyon.y <= 650 and farePozisyon.y > 450) ){
		if (durum[2][1] == 0){
			durum[2][1] = kulaniciId;
			return true;
		}
	}
	else if ( (farePozisyon.x < 600 and farePozisyon.x > 400) and (farePozisyon.y <= 650 and farePozisyon.y > 450) ){
		if (durum[2][2] == 0){
			durum[2][2] = kulaniciId;
			return true;
		}		
	}
	return false;
}
void elSifirla(short durumMatrisi[3][3]){
	for (short i = 0; i < 3; i++)
	{
		for(short j = 0; j < 3; j++)
		{
			durumMatrisi[i][j] = 0;
		}
	}
}

bool bitisKontrol(short durumMatrisi[3][3], short kullaniciIdParam){ // giriş yapıldıktan sonra bitisKontrol kullanılacak
	if (durumMatrisi[0][0] == kullaniciIdParam and durumMatrisi[0][1] == kullaniciIdParam and durumMatrisi[0][2] == kullaniciIdParam){
		return true; // 1.Satır 
	}
	if (durumMatrisi[1][0] == kullaniciIdParam and durumMatrisi[1][1] == kullaniciIdParam and durumMatrisi[1][2] == kullaniciIdParam){
		return true; // 2.Satır 	
	}
	if (durumMatrisi[2][0] == kullaniciIdParam and durumMatrisi[2][1] == kullaniciIdParam and durumMatrisi[2][2] == kullaniciIdParam){
		return true; // 3.Satır 
	}
	if (durumMatrisi[0][0] == kullaniciIdParam and durumMatrisi[1][0] == kullaniciIdParam and durumMatrisi[2][0] == kullaniciIdParam){
		return true; // 1.Sutun
	}
	if (durumMatrisi[0][1] == kullaniciIdParam and durumMatrisi[1][1] == kullaniciIdParam and durumMatrisi[2][1] == kullaniciIdParam){
		return true; // 2.Sutun
	}
	if (durumMatrisi[0][2] == kullaniciIdParam and durumMatrisi[1][2] == kullaniciIdParam and durumMatrisi[2][2] == kullaniciIdParam){
		return true; // 3.Sutun
	}
	if (durumMatrisi[0][0] == kullaniciIdParam and durumMatrisi[1][1] == kullaniciIdParam and durumMatrisi[2][2] == kullaniciIdParam){
		return true; // Sol üstten - sağ alta çapraz
	}
	if (durumMatrisi[2][0] == kullaniciIdParam and durumMatrisi[1][1] == kullaniciIdParam and durumMatrisi[0][2] == kullaniciIdParam){
		return true; // Sag üstten - sol alta çapraz
	}
	return false;
}
bool berabereKontrol(){
	bool berabere = true;
	for (short i = 0; i < 3; i++)
	{
		for(short j = 0; j < 3; j++)
		{
			if (durum[i][j] == 0){
				berabere = false;
			}
		}
	}
	return berabere;
}
sf::Text xCiz(float x, float y){
	sf::Text metinX;
		metinX.setFont(font);
		metinX.setPosition(x,y);
		metinX.setFillColor(sf::Color::White);
		metinX.setCharacterSize(hucreBoyutu);
		metinX.setString("X");
	return metinX;
}
sf::Text oCiz(float x, float y){
	sf::Text metinO;
		metinO.setFont(font);
		metinO.setPosition(x,y);
		metinO.setFillColor(sf::Color::White);
		metinO.setCharacterSize(hucreBoyutu);
		metinO.setString("O");
	return metinO;
}
sf::Text temizle(float x, float y, string metin){
	sf::Text metinTemizle;
		metinTemizle.setFont(font);
		metinTemizle.setPosition(x,y);
		metinTemizle.setFillColor(sf::Color::Black);
		metinTemizle.setCharacterSize(hucreBoyutu);
		metinTemizle.setString(metin);
	return metinTemizle;
}
sf::Font fontYukle(string fontYolu){
	sf::Font fontNeuropol;
		if(!fontNeuropol.loadFromFile(fontYolu)){
			cout<<"!#-> Font Yüklenemedi!!";
		}
	return fontNeuropol;
}
