#include <cstring>
#include <iostream>
#include <sstream>
#include <assert.h>

/*
 * 5_2. Современники.
Группа людей называется современниками если был такой момент,
когда они могли собраться вместе. Для этого в этот момент каждому из них должно было
уже исполниться 18 лет, но ещё не исполниться 80 лет.
Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников.
В день 18летия человек уже может принимать участие в собраниях, а в день 80летия и в день смерти уже не может.
Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия. В этих случаях принимать участие в собраниях он не мог.


 *
 * */

class TaskDate {
 public:
  int status;
  int day;
  int month;
  int year;
  TaskDate();
  TaskDate(const int, const int, const int);
  TaskDate operator=(const TaskDate &&);
  TaskDate operator=(const TaskDate &);

 private:
  TaskDate(const TaskDate &&);
  TaskDate(const TaskDate &);
};

TaskDate TaskDate::operator=(const TaskDate &&b) {
  status = b.status;
  year = b.year;
  month = b.month;
  day = b.day;
}

TaskDate TaskDate::operator=(const TaskDate &b) {
  status = b.status;
  year = b.year;
  month = b.month;
  day = b.day;
}

TaskDate::TaskDate() : year(0), month(0), day(0), status(0) {}

TaskDate::TaskDate(const int _day, const int _month, const int _year) : day(_day), month(_month) , year(_year) {
}

bool isLessDate(const TaskDate &x1, const TaskDate &x2) {
  if (x1.year == x2.year) {
    if (x1.month == x2.month) {
      if (x1.day == x2.day) return x1.status > x2.status;
      return x1.day < x2.day;
    }
    return x1.month < x2.month;
  }
  return x1.year < x2.year;
}

template <class T>
void mergeSort(T * arr, size_t len, bool(isLess)(const T &, const T &)) {

  for(int size=1; size < len; size= size * 2 )
  {
    T *temp = new T[len];
    int l1=0;
    int k=0;
    while( l1+size < len)
    {
      int h1=l1+size-1;
      int l2=h1+1;
      int h2=l2+size-1;

      if(h2 >= len )
        h2= len - 1;


      int i=l1;
      int j=l2;

      while(i<=h1 && j<=h2 )
      {
        if(isLess( arr[i], arr[j]) )
          temp[k++]=arr[i++];
        else
          temp[k++]=arr[j++];
      }

      while(i<=h1)
        temp[k++]=arr[i++];
      while(j<=h2)
        temp[k++]=arr[j++];

      l1=h2+1;
    }

    memcpy(&temp[k],&arr[l1], (len - l1) * sizeof(T));

    memcpy(arr,temp,len*sizeof(T));
    delete[] temp;
      }
}

void run(std::istream& input,std::ostream& output) {
  int size;
  const int YOUNG = 18, OLD = 80;
  input >> size;
  size *= 2;
  TaskDate *pDate = new TaskDate[size];
  for (int i = 0; i < size; i += 2) {
    input >> pDate[i].day >> pDate[i].month >> pDate[i].year;
    input >> pDate[i + 1].day >> pDate[i + 1].month >> pDate[i + 1].year;
    if (isLessDate(TaskDate(pDate[i].day,pDate[i].month,pDate[i].year + YOUNG), pDate[i + 1])) {
      pDate[i + 1].status = OLD;
    pDate[i].status = YOUNG;
    if (isLessDate(TaskDate(pDate[i].day,pDate[i].month,pDate[i].year + OLD), pDate[i + 1])) {
      pDate[i + 1].year = pDate[i].year + OLD;
      pDate[i + 1].day = pDate[i].day;
      pDate[i + 1].month = pDate[i].month;
    }
    pDate[i].year += YOUNG;
  }
  }

  mergeSort(pDate, size, isLessDate);
  int max_count = 0;
  int count = 0;
  for (int j = 0; j < size; ++j) {
    if (pDate[j].status == YOUNG) count++;
    if (pDate[j].status == OLD) count--;
    if (count > max_count) max_count = count;
  }


  output << max_count;
  delete[] pDate;
}

void test() {
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 2 5 1980 13 11 2055 1 1 1982 1 1 2030 2 1 1920 2 1 2000";
    run(input,output);
    assert(output.str() == "3");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "4 2 5 1980 13 11 2055 1 1 1982 1 1 2030 2 1 1920 2 1 2000 2 1 1920 2 1 1938";
    run(input,output);
    assert(output.str() == "3");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "4 2 5 1980 13 11 2055 1 1 1982 1 1 2030 2 1 1920 1 1 2000 2 1 1920 2 1 1938";
    run(input,output);
    assert(output.str() == "2");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 2 5 1980 1 1 2000 1 1 1982 1 1 2030 1980 1 1 2000 1 1 1982 1 1 2030";
    run(input,output);
    assert(output.str() == "1");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "2 2 5 1980 1 1 2000 1 1 1982 1 1 2030";
    run(input,output);
    assert(output.str() == "1");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 2 5 1980 2 1 2000 1 1 2082 1 1 2130 1 1 1982  1 2 2100";
    run(input,output);
    assert(output.str() == "2");
  }
  {
    std::stringstream input;
    std::stringstream output;
    input << "3 2 5 1980 2 1 2000 1 1 1980 1 1 2130 1 1 1982 1 2 2100";
    run(input,output);
    assert(output.str() == "3");
  }
//  {
//    std::stringstream input;
//    std::stringstream output;
//    input << "3 2 5 1980 2 1 2000 1 1 1980 1 1 2130 1 1 1982 1 1 2000";
//    run(input,output);
//    assert(output.str() == "2");
//  }
}

int main() {
  run(std::cin, std::cout);
  //test();
  return 0;
}
