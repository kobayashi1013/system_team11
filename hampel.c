#include <stdio.h>
#include <math.h>

#define dataNum 100
#define hampel_window 5
#define hampel_diff 3

void setData(void);
void drawData(int _value);
void shiftWindow(int _value);
int filter(int _value);
void sort(int _array[], const int _arrayNum);
int center(const int _array[], const int _arrayNum);
float average(const int _array[], const int _arrayNum);
float dispersion(const int _array[], const int _arrayNum);

int data[dataNum];
int window[hampel_window];

int main(void)
{
	setData();

	//printf("%f\n", sqrt(dispersion(data, dataNum)));
	for (int i = 0; i < dataNum; i++)
	{
		if (i >= hampel_window)
		{
			data[i] = filter(data[i]);
			drawData(window[hampel_window - 1]);

			shiftWindow(data[i]);
		}
		else
		{
			window[i] = data[i];
		}
	}

	return 0;
}

void setData(void)
{
	for (int i = 0; i < dataNum; i++)
	{
		data[i] = i;
	}

	data[25] = 1000;
	data[50] = -1500;
	data[51] = 1200;
}

void drawData(int _value)
{
	printf("%d\n", _value);
}

void shiftWindow(int _value)
{
	for (int i = 0; i < hampel_window - 1; i++)
	{
		window[i] = window[i + 1];
	}

	window[hampel_window - 1] = _value;
}

int filter(int _value)
{
	int _center = center(window, hampel_window);
	float _deviation = sqrt(dispersion(window, hampel_window));

	float _upper = _center + hampel_diff * _deviation;
	float _lower = _center - hampel_diff * _deviation;

	if (_value > _upper || _value < _lower)
	{
		_value = _center;
	}

	return _value;
}

void sort(int _array[], const int _arrayNum)
{
	for (int i = 0; i < _arrayNum; i++)
    {
        for (int j = i + 1; j < _arrayNum; j++)
        {
            if (_array[i] > _array[j])
            {
                int _tmp = _array[i];
                _array[i] = _array[j];
                _array[j] = _tmp;
            }
        }
    }
}

int center(const int _array[], const int _arrayNum)
{
	int _tmpArray[_arrayNum];

	for (int i = 0; i < _arrayNum; i++)
	{
		_tmpArray[i] = _array[i];
	}

	sort(_tmpArray, _arrayNum);

	int _center = 0;

	if (_arrayNum % 2 == 1)
	{
		_center = (int)_tmpArray[_arrayNum / 2];
	}
	else
	{
		_center = (int)((_tmpArray[_arrayNum / 2] + _tmpArray[_arrayNum / 2 + 1]) / 2);
	}

	return _center;
}

float average(const int _array[], const int _arrayNum)
{
	int _sigma = 0;

	for (int i = 0; i < _arrayNum; i++)
	{
		_sigma += _array[i];
	}

	return _sigma / (float)_arrayNum;
}

float dispersion(const int _array[], const int _arrayNum)
{
	float _average = average(_array, _arrayNum);
	float _sigma;

	for (int i = 0; i < _arrayNum; i++)
	{
		_sigma += (_array[i] - _average) * (_array[i] - _average);
	}

	return _sigma / _arrayNum;
}
