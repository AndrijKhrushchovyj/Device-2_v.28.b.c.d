#include "header.h"

/*****************************************************/
//Формуємо екран відображення стану лічильників ресурсу
/*****************************************************/
void make_ekran_resurs()
{
  static unsigned char const name_string[MAX_NAMBER_LANGUAGE][MAX_ROW_FOR_EKRAN_RESURS][MAX_COL_LCD] = 
  {
    {
      "  Ресурс выкл.  ",
      " Кол.отключений "
    },
    {
      "Ресурс вимикача ",
      " Кільк.вимикань "
    },
    {
      "  CB Resource   ",
      " Open CB Oper N "
    },
    {
      "  Ресурс выкл.  ",
      " Кол.отключений "
    }
  };
  int index_language = index_language_in_array(current_settings.language);
  
  unsigned int position_temp = current_ekran.index_position;
  unsigned int index_of_ekran;
  
  //Множення на два величини position_temp потрібне для того, бо наодн позицію ми використовуємо два рядки (назва + значення)
  index_of_ekran = ((position_temp<<1) >> POWER_MAX_ROW_LCD) << POWER_MAX_ROW_LCD;

  unsigned int col_begin = 0, col_end = MAX_COL_LCD - 1;
  for (unsigned int i=0; i< MAX_ROW_LCD; i++)
  {
    unsigned int index_of_ekran_tmp = index_of_ekran >> 1;
    if (index_of_ekran_tmp < MAX_ROW_FOR_EKRAN_RESURS)
    {
      if ((i & 0x1) == 0)
      {
        //У непарному номері рядку виводимо заголовок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = name_string[index_language][index_of_ekran_tmp][j];
      }
      else
      {
        //У парному номері рядку виводимо значення

        //Очищаємо рядок
        for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';
        
        unsigned int value;
        if (index_of_ekran_tmp == INDEX_ML_RESURS_CURRENT)
          value = resurs_vymykacha;
        else
          value = resurs_vidkljuchennja;

        unsigned int vaga = 1; //початковий ваговий коефіцієнт
        unsigned int number_digits = 1; //початкова кількість розрядів
        unsigned int first_symbol = 0; //помічаємо, що ще ні один значущий символ не виведений

        //підраховуємо кількість значущих чисел
        unsigned int divisor = 10;
        unsigned int value_tmp = value;
        do
        {
          value_tmp /= divisor;
          if (value_tmp != 0)
          {
            number_digits++;
            vaga *= 10;
          }
        }
        while (value_tmp != 0 );

        if (value == 0xffffffff)
        {
          //Перед числом будемо добавляти знак більше-рівне ">="
          col_begin = (MAX_COL_LCD - (number_digits + 2)) >> 1;
          col_end = col_begin +  (number_digits + 2) - 1;
          
          //Позиція першого числа  (іде після знаку більше-рівне ">=")
          col_begin += 2;
         
        }
        else
        {
          col_begin = (MAX_COL_LCD - number_digits) >> 1;
          col_end = col_begin +  number_digits - 1;
        }

        value_tmp = value;
        for (unsigned int j = 0; j<MAX_COL_LCD; j++)
        {
          if ((j >= col_begin) &&  (j <= col_end ))
            calc_int_symbol_and_put_into_working_ekran((working_ekran[i] + j), &value, &vaga, &first_symbol, true);
        }
        
        if (value_tmp == 0xffffffff)
        {
          working_ekran[i][col_begin - 2] = '>';
          working_ekran[i][col_begin - 1] = '=';
          
          col_begin -= 2; //Ця змінна ще потрібна для виведення курсору
        }
      }
    }
    else
      for (unsigned int j = 0; j<MAX_COL_LCD; j++) working_ekran[i][j] = ' ';

    index_of_ekran++;
  }

  //Відображення курору по вертикалі і курсор завжди має бути у полі із значенням устаки
  current_ekran.position_cursor_y = ((position_temp<<1) + 1) & (MAX_ROW_LCD - 1);
  //Курсор по горизонталі відображається на першому символі у випадку, коли ми не в режимі редагування, інакше позиція буде визначена у функцї main_manu_function
  if (current_ekran.edition == 0)
  {
    int last_position_cursor_x = col_end;
    current_ekran.position_cursor_x = col_begin;

    //Підтягуємо курсор до першого символу
    while (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x + 1]) == ' ') && 
           (current_ekran.position_cursor_x < (last_position_cursor_x -1))) current_ekran.position_cursor_x++;
    
    //Курсор ставимо так, щоб він був перед числом
    if (((working_ekran[current_ekran.position_cursor_y][current_ekran.position_cursor_x]) != ' ') && 
        (current_ekran.position_cursor_x > 0)) current_ekran.position_cursor_x--;
  }
  //Курсор видимий
  current_ekran.cursor_on = 0;
  //Курсор не мигає
  /*if(current_ekran.edition == 0)*/current_ekran.cursor_blinking_on = 0;
  /*else current_ekran.cursor_blinking_on = 1;*/
  //Обновити повністю весь екран
  current_ekran.current_action = ACTION_WITH_CARRENT_EKRANE_FULL_UPDATE;
}
/*****************************************************/


/*****************************************************/
//
/*****************************************************/
/*****************************************************/
