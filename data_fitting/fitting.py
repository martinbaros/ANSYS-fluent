import numpy as np
import pandas as pd
from scipy.optimize import curve_fit
from matplotlib import pyplot as plt
from sklearn.metrics import r2_score
from scipy.interpolate import UnivariateSpline

# Load the data from CSV
df = pd.read_csv('data_fitting/data.csv')  # Make sure to replace 'path_to_your_file.csv' with your actual file path

# Extract the Z column as x-data and Velocity as y-data
x = df[' Y [ m ]'].values
y = df[' Velocity [ m s^-1 ]'].values


def four_parameter_logistic(x, A, B, C, D):
    return (A - D) / (1.0 + (x / C)**B) + D

def five_parameter_logistic(x, A, B, C, D, E):
    return D + (A - D) / ((1 + (x / C)**B)**E)

def gaussian(x, A, B, C):
    return A * np.exp(-((x - B)**2) / (2 * C**2))

def michaelis_menten(x, Vmax, Km):
    return (Vmax * x) / (Km + x)

def linear(x, a, b):
    return a * x + b

def quadratic(x, a, b, c):
    return a * x**2 + b * x + c

def cubic(x, a, b, c, d):
    return a * x**3 + b * x**2 + c * x + d

def quartic(x, a, b, c, d, e):
    return a * x**4 + b * x**3 + c * x**2 + d * x + e

def quintic(x, a, b, c, d, e, f):
    return a * x**5 + b * x**4 + c * x**3 + d * x**2 + e * x + f

def exponential(x, a, b):
    return a * np.exp(b * x)

def logarithmic(x, a, b):
    return a * np.log(x + 1) + b  # x+1 to avoid log(0)

def power(x, a, b):
    if b <= 0:
        return np.inf
    return a * np.power(x, b)

# List of functions to try
functions = [linear, quadratic, cubic, quartic, exponential, logarithmic, power]
function_names = ['Linear', 'Quadratic', 'Cubic', 'Quartic', 'Exponential', 'Logarithmic', 'Power']

functions.extend([four_parameter_logistic, five_parameter_logistic, gaussian, michaelis_menten])
function_names.extend(['4-Parameter Logistic', '5-Parameter Logistic', 'Gaussian', 'Michaelis-Menten'])

# Initialize the best fit tracking variables
best_fit = None
best_r2 = -np.inf  # Very low initial R-squared
best_func_name = ''
best_params = None

# Attempt to fit each function to the data
for func, name in zip(functions, function_names):
    try:
        # Use sensible initial guesses for complex functions where needed
        if name in ['4-Parameter Logistic', '5-Parameter Logistic', 'Gaussian', 'Michaelis-Menten']:
            # Example of generic initial guesses, may need adjustment
            initial_guesses = {
                '4-Parameter Logistic': [1, 1, np.median(x), 1],
                '5-Parameter Logistic': [1, 1, np.median(x), 1, 1],
                'Gaussian': [1, np.mean(x), 1],
                'Michaelis-Menten': [np.max(y), np.median(x)]
            }
            params, _ = curve_fit(func, x, y, p0=initial_guesses[name], maxfev=10000)
        else:
            params, _ = curve_fit(func, x, y, maxfev=10000)
        
        # Calculate the R-squared value
        y_pred = func(x, *params)
        r2 = r2_score(y, y_pred)
        
        print(f"{name} fit: R-squared = {r2:.4f}")
        
        # Update the best fitting model if necessary
        if r2 > best_r2:
            best_r2 = r2
            best_fit = y_pred
            best_func_name = name
            best_params = params
    except Exception as e:
        print(f"Could not fit {name} due to: {e}")

# Incorporate and compare UnivariateSpline fitting
spline_fit = UnivariateSpline(x, y, s=len(x))
y_spline_fit = spline_fit(x)
spline_r2 = r2_score(y, y_spline_fit)

print(f"Spline fit: R-squared = {spline_r2:.4f}")

if spline_r2 > best_r2:
    best_r2 = spline_r2
    best_fit = y_spline_fit
    best_func_name = 'Spline'
    # Spline does not return simple parameters like curve_fit

print(f"Best fitting function: {best_func_name} with R-squared = {best_r2}")
print(f"Best parameters: {best_params}")
# Plot the best fitting function along with the data
plt.plot(x, y, 'o', color ='red', label ="data")
plt.plot(x, best_fit, '--', color ='blue', label =f"Best fit: {best_func_name}")
plt.legend()
plt.show()

