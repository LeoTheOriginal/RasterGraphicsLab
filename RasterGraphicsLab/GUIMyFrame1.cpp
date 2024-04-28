#include "GUIMyFrame1.h"
#include <cmath>

GUIMyFrame1::GUIMyFrame1(wxWindow* parent) : MyFrame1(parent)
{
	m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
	m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
	this->SetBackgroundColour(wxColor(192, 192, 192));
	m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
	m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));
}

wxImage GUIMyFrame1::CopyImage() {
	return Img_Org.Copy();
}

void GUIMyFrame1::m_scrolledWindow_update(wxUpdateUIEvent& event)
{
	Repaint();
}

void GUIMyFrame1::m_b_grayscale_click(wxCommandEvent& event)
{
	// TO DO: Konwersja do skali szarosci
	Img_Cpy = CopyImage().ConvertToGreyscale();
	Repaint();
}

void GUIMyFrame1::m_b_blur_click(wxCommandEvent& event)
{
	// TO DO: Rozmywanie obrazu (blur)
	Img_Cpy = CopyImage().Blur(10);
	Repaint();
}

void GUIMyFrame1::m_b_mirror_click(wxCommandEvent& event)
{
	// TO DO: Odbicie lustrzane
	Img_Cpy = CopyImage().Mirror();
	Repaint();
}

void GUIMyFrame1::m_b_replace_click(wxCommandEvent& event)
{
	// TO DO: Zamiana kolorow
	Img_Cpy = CopyImage();
	Img_Cpy.Replace(254, 0, 0, 0, 0, 255);
	Repaint();
}

void GUIMyFrame1::m_b_rescale_click(wxCommandEvent& event)
{
	// TO DO: Zmiana rozmiarow do 320x240

	wxClientDC DC1(m_scrolledWindow);
	wxBufferedDC DC(&DC1);
	m_scrolledWindow->ClearBackground();
	Img_Cpy = CopyImage().Rescale(320, 240);
	DC.Clear();
}

void GUIMyFrame1::m_b_rotate_click(wxCommandEvent& event)
{
	// TO DO: Obrot o 30 stopni
	Img_Cpy = CopyImage().Rotate(30.0 * M_PI / 180.0, wxPoint(Img_Org.GetSize().x / 2, Img_Org.GetSize().y / 2));
	Repaint();
}

void GUIMyFrame1::m_b_rotate_hue_click(wxCommandEvent& event)
{
	// TO DO: Przesuniecie Hue o 180 stopni
	Img_Cpy = CopyImage();
	Img_Cpy.RotateHue(0.5);
	Repaint();
}

void GUIMyFrame1::m_b_mask_click(wxCommandEvent& event)
{
	// TO DO: Ustawienie maski obrazu
	wxClientDC DC1(m_scrolledWindow);
	wxBufferedDC DC(&DC1);
	Img_Cpy = CopyImage();
	Img_Cpy.SetMaskFromImage(Img_Mask, 0, 0, 0);
	DC.Clear();
}

void GUIMyFrame1::m_s_brightness_scroll(wxScrollEvent& event)
{
	// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
	// Brightness(...), ktora zmienia jasnosc. W tym miejscu nic nie
	// zmieniamy. Do uzupelnienia pozostaje funkcja Brightness(...)
	Brightness(m_s_brightness->GetValue() - 100);
	Repaint();
}

void GUIMyFrame1::m_s_contrast_scroll(wxScrollEvent& event)
{
	// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
	// Contrast(...), ktora zmienia kontrast. W tym miejscu nic nie
	// zmieniamy. Do uzupelnienia pozostaje funkcja Contrast(...)
	Contrast(m_s_contrast->GetValue() - 100);
	Repaint();
}

void GUIMyFrame1::m_b_prewitt_click(wxCommandEvent& event)
{
	// TO DO: Pionowa maska Prewitta

	Img_Cpy = CopyImage();

	int temp = 0;
	int width = Img_Cpy.GetSize().x;
	int height = Img_Cpy.GetSize().y;
	int data = width * height * 3;

	unsigned char* point = Img_Cpy.GetData();
	unsigned char* back = new unsigned char[data];

	for (int i = 0; i < data; i++)
		back[i] = point[i];
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				temp = 0;
				temp += back[k + i * 3 * width + 3 * (j + 1)];
				temp -= back[k + i * 3 * width + 3 * (j - 1)];
				temp += back[k + (i + 1) * 3 * width + 3 * (j + 1)];
				temp -= back[k + (i + 1) * 3 * width + 3 * (j - 1)];
				temp += back[k + (i - 1) * 3 * width + 3 * (j + 1)];
				temp -= back[k + (i - 1) * 3 * width + 3 * (j - 1)];
				point[k + i * 3 * width + 3 * j] = fabs(temp) / 3.0;
			}
		}
	}

	delete[] back;

}

void GUIMyFrame1::m_b_threshold_click(wxCommandEvent& event)
{
	// TO DO: Prog o wartosci 128 dla kazdego kanalu niezaleznie
	Img_Cpy = CopyImage();
	unsigned char* point = Img_Cpy.GetData();
	int data = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;
	for (int i = 0; i < data; i++) {
		point[i] = (point[i] < 128) ? 0 : 255;
	}
}


void GUIMyFrame1::Contrast(int value)
{
	// TO DO: Zmiana kontrastu obrazu. value moze przyjmowac wartosci od -100 do 100

	 double c = (100.0 + value) / (101.0 - value);
    Img_Cpy = CopyImage();
    unsigned char* point = Img_Cpy.GetData();
    int data = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;
    for (int i = 0; i < data; i++) {
        double temp = c * (point[i] - 127) + 127;
        point[i] = std::max(0, std::min(255, static_cast<int>(temp)));
    }
}

void GUIMyFrame1::Repaint()
{
	wxBitmap bitmap(Img_Cpy);          // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
	wxClientDC dc(m_scrolledWindow);   // Pobieramy kontekst okna
	m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
	dc.DrawBitmap(bitmap, 0, 0, true); // Rysujemy bitmape na kontekscie urzadzenia
}

void GUIMyFrame1::Brightness(int value)
{
	// TO DO: Zmiana jasnosci obrazu. value moze przyjmowac wartosci od -100 do 100
	Img_Cpy = CopyImage();
	unsigned char* point = Img_Cpy.GetData();
	int data = Img_Cpy.GetSize().x * Img_Cpy.GetSize().y * 3;
	for (int i = 0; i < data; i++) {
		int temp = point[i] + value;
		point[i] = std::max(0, std::min(255, temp));
	}
}