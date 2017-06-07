"""
    Testing of activation functions
    Due to a weak mind, I did not think of anything better 
        than simply drawing graphics of activation functions and their derivatives.
"""


import os
import re
import numpy as np
import pylab as pl


def select(r, xs):
    """ возвращает список, из найденных груп в исходном списке
    :param r: регулярка
    :param xs: список строк для парсинга """
    return [m.group() for m in (re.match(r, x) for x in xs) if m]


def data(filename):
    list = []
    file = open(filename, 'r')
    for line in file:
        list.append(float(line))
    return np.array(list)


def name_param(filename):
    r = re.search(r'(fnc_|der_)(\S*)\(h=(\d*)\)\.txt', filename)
    if r == None:
        return None
    nh = r.groups()
    return nh[1], float(nh[2])


def max_distance(sequence):
    length = len(sequence)
    md = 0
    for i in range(length - 1):
        if sequence[i+1] - sequence[i] > md:
            md = sequence[i+1] - sequence[i]
    return md


def RK4(f):
    return lambda t, y, h: (
        lambda dy1: (
            lambda dy2: (
                lambda dy3: (
                    lambda dy4: (dy1 + 2 * dy2 + 2 * dy3 + dy4) / 6
                )(h * f(t + h, y + dy3))
            )(h * f(t + h / 2, y + dy2 / 2))
        )(h * f(t + h / 2, y + dy1 / 2))
    )(h * f(t, y))


def view(X, fnc_der_filenames, fncname, param):
    if param == 2.0:
        a = 0
    fnc_file = fnc_der_filenames[0]
    der_file = fnc_der_filenames[1]
    report = ""

    Y, dY = [0], [0]

    length = len(X)

    if fnc_file == None:
        report += "    Only the points of the derivative are calculated for this function\n"
    else:
        Y = data(fnc_file)
        if len(Y) != length:
            report += "    Mismatch of the number of points of the argument and the function\n"

    if der_file == None:
        report += "    Only the points of the function are calculated\n"
    else:
        dY = data(der_file)
        if len(dY) != length:
            report += "    Mismatch of the number of points of the argument and the derivative\n"

    os.chdir('plots/')

    _Y_ = _dY_ = [0 for i in range(length)]
    _Y_[0] = Y[0]
    _dY_[0] = dY[0]

    if len(dY) == length:
        for i in range(1, length):
            dy = RK4(lambda t, y: dY[i-1])
            _Y_[i] = _Y_[i-1] + dy(X[i-1], _Y_[i-1], X[i] - X[i-1])
        pl.figure()
        if len(Y) == length:
            pl.plot(X, Y, 'r-')
            error = 0
            for i in range(length):
                if abs(Y[i] - _Y_[i]) > error:
                    error = abs(Y[i] - _Y_[i])
            report += "    integration error = " + str(error) + '\n'
        pl.plot(X, _Y_, 'g-')
        pl.title(fncname + "  h = " + str(param))
        pl.grid()
        pl.savefig("fnc_" + fncname + "(h=" + str(param) + ").png")

    if len(Y) == length:
        for i in range(length-1):
            _dY_[i] = (Y[i+1] - Y[i]) / (X[i+1] - X[i])
        _dY_[length-1] = _dY_[length-2]
        pl.figure()
        if len(dY) == length:
            pl.plot(X, dY, 'r-')
            error = 0
            for i in range(length):
                if abs(dY[i] - _dY_[i]) > error:
                    error = abs(dY[i] - _dY_[i])
            report += "    derivative error = " + str(error) + '\n'
        pl.plot(X, _dY_, 'g-')
        pl.title("derivative of " + fncname + "  h = " + str(param))
        pl.grid()
        pl.savefig("der_" + fncname + "(h=" + str(param) + ").png")

    os.chdir('../')

    return report


program = __name__
while program == '__main__':

    program = "not name"
    path = "../Activation_Function/"
    os.chdir("../Activation_Function/")

    report = open('report.txt', 'w')

    listdir = os.listdir()
    print(listdir)

    report.write("received files:\n")
    for filename in sorted(listdir):
        report.write("    " + filename + '\n')

    report.write("\n\n")

    X = data(path + 'X.txt')
    if len(X) == 0:
        report.write("X-vector has not found :(")
        break

    report.write("len(X) = " + str(len(X)) + '\n')
    report.write("X = [ " + str(X[0]) + " ,  " + str(X[len(X)-1]) + " ]\n")
    report.write("max interval of X-vector : " + str(max_distance(X)) + '\n')

    report.write("================================================\n")

    fnc_filelist = select(r'fnc_\S*', listdir)
    der_filelist = select(r'der_\S*', listdir)

    F = {name_param(filename): path + filename for filename in fnc_filelist if name_param(filename) != None}
    D = {name_param(filename): path + filename for filename in der_filelist if name_param(filename) != None}

    pairs = {key: (F.get(key), D.get(key)) for key in set(F.keys()) | set(D.keys())}

    if not os.path.exists('plots/'):
        os.mkdir('plots')


    for key in reversed(sorted(pairs.keys())):
        print(key, " :  ", pairs[key])
        report.write(key[0] + " , param = " + str(key[1]) + " : \n")
        report.write(view(X, pairs[key], key[0], key[1]))