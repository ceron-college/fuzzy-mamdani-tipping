#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Different types of membership functions that can be used
// in fuzzy sets
enum MFType
{
  TRIANG, // Triangular membership function type
  TRAP,   // Trapezoidal membership function type
  SAT,    // Saturation membership function type
  GAUSS   // Gaussian membership function type
};

/******* Membership Functions *******/

// Triangular membership function for a fuzzy set
// 4 parameters, representing the left, center, and right boundaries of the triangle
// X is the value for which the function will be evaluated
double triangmf(double left, double center, double right, double x)
{

  // Initialize the variable that will store the result of the calculation
  double res = 0;

  // If x is less than or equal to the left value, it does not belong to the set
  if (x <= left)
    // The membership degree is 0
    res = 0;
  // If x is greater than or equal to the right value, it does not belong to the set
  else if (x >= right)
    // The membership degree is 0
    res = 0;
  // If x is within the range of the center and the left value, calculate the
  // membership degree with the ascending slope formula.
  else if (x > left && x <= center)
    res = (x - left) / (center - left);
  // If x is within the range of the center and the right value, calculate the
  // membership degree with the descending slope formula.
  else if (x < right && x > center)
    res = 1 - abs((right - x) / (center - right));

  return res; // Return the calculated membership degree
}

// Trapezoidal membership function
// 5 parameters, representing the lower left, upper left, upper right, and lower right boundaries of the trapezoid
// x is the value for which the membership function will be evaluated
double trapmf(double lowLeft, double upLeft, double upRight, double lowRight,
              double x)
{
  // Initialize the variable that will store the result of the calculation
  double res = 0;

  // If x is less than or equal to the lower left value, it does not belong to the set
  if (x <= lowLeft)
    // The membership degree is 0
    res = 0;
  // If x is greater than or equal to the lower right value, it does not belong to the set
  else if (x >= lowRight)
    // The membership degree is 0
    res = 0;
  // If x is within the range of the left side, calculate the membership degree
  // using the ascending slope formula.
  else if (x > lowLeft && x <= upLeft)
    res = (x - lowLeft) / (upLeft - lowLeft);
  // If x is within the upper left range, the membership degree is 1
  else if (x > upLeft && x <= upRight)
    res = 1;
  // If x is within the upper right range, calculate the membership degree
  // using the descending slope formula.
  else if (x > upRight && x < lowRight)
    res = 1 - abs((upRight - x) / (lowRight - upRight));

  return res; // Return the calculated membership degree
}

// Saturation membership function for a fuzzy set
// 3 parameters, representing the upper and lower limits
// x is the value for which the membership function will be evaluated
double satmf(double up, double down, double x)
{

  // Initialize the variable that will store the result of the calculation
  double res = 0;

  // Check if the region is to the left or right
  if (up < down)
  {
    // If x is less than or equal to the upper limit, the membership degree is 1
    if (x <= up)
      res = 1;
    // If x is greater than or equal to the lower limit, the membership degree is 0
    else if (x >= down)
      res = 0;
    // Calculate the membership degree for x values within the saturation range
    else
      res = 1 - abs((up - x) / (down - up));
  }
  // If the saturation is to the right
  else
  {
    // If x is greater than or equal to the upper limit, the membership degree is 1
    if (x >= up)
      res = 1;
    // If x is less than or equal to the lower limit, the membership degree is 0
    else if (x <= down)
      res = 0;
    // Calculate the membership degree for x values within the saturation range
    else
      res = (x - down) / (up - down);
  }

  return res; // Return the calculated membership degree
}

// Gaussian membership function
// 3 parameters, representing the center of the function and the width of the bell curve
// x is the value for which the membership function will be evaluated
double gaussianmf(double center, double width, double x)
{

  // Calculate the membership degree using the Gaussian function and return the result
  return exp(
      // Calculate the distance between x and the center of the function
      // Calculate the square root of twice the width of the bell curve
      // Square the previous result to get the exponent
      // Calculate the value of the exponential function
      -pow((x - center) / (sqrt(2 * width)), 2));
}

/******* Norms *******/
// Implement the AND operation in vector form
// Takes a vector of fuzzy arguments represented as membership values
// Returns the result of the AND operation
double fAnd(vector<double> &args)
{

  // Initialize the variable with the first value of the vector
  int minimum = args.front();

  // Iterate over all elements of the vector
  for (int i = 0; i < args.size(); i++)
  {
    // Compare the current vector value with the minimum found
    // If it's smaller, update the minimum value
    if (minimum > args[i])
      minimum = args[i];
  }

  return minimum; // Return the minimum value found in the vector
}

// Implement the AND operation for two fuzzy arguments
// a and b represent the membership degrees of two fuzzy sets
double fAnd(double a, double b)
{
  // Min calculates the minimum between a and b
  return min(a, b); // Return the result of the AND operation
}

// Implement the OR operation in vector form
// Takes a vector of fuzzy arguments represented as membership values
double fOr(const vector<double> &args)
{

  // Initialize the variable with the first value of the vector
  double maximum = args.front();

  // Iterate over all elements of the vector
  for (size_t i = 1; i < args.size(); ++i)
  {
    // Compare the current vector value with the maximum found
    // If the current value is greater, update the maximum value
    if (maximum < args[i])
      maximum = args[i];
  }

  return maximum; // Return the maximum value found
}

// Implement the OR operation for two fuzzy arguments
// Takes 2 fuzzy arguments, representing membership values of 2 fuzzy sets
// Returns the result of the OR operation
double fOr(double a, double b)
{
  // Max calculates the maximum between a and b, representing the resulting membership degree
  return max(a, b); // Return the maximum between a and b
}

// Class to represent a fuzzy set
class FuzzySet
{
protected:
  string name;           // Name of the fuzzy set
  MFType type;           // Type of membership function
  vector<double> params; // Stores the parameters of the membership function

public:
  FuzzySet(string n)
      : name(n) {} // Constructor that initializes the name of the fuzzy set
  virtual ~FuzzySet()
  {
  } // Virtual destructor to destroy derived classes

  // Virtual method to evaluate the membership degree of an input value x
  virtual double eval(double x) const = 0;

  // Method to get the name of the fuzzy set
  string getName() const { return name; }

  // Method to set the type of membership function and its parameters
  void setMF(MFType t, vector<double> &args)
  {
    type = t;
    params = args;
  }
};

// Class representing an input fuzzy set
// Inherits from the FuzzySet class
class InputFuzzySet : public FuzzySet
{
private:
  map<string, double> membershipValues; // Map to store calculated membership values

public:
  InputFuzzySet(string n)
      : FuzzySet(n)
  {
  } // Constructor that initializes the name of the fuzzy set

  // Method to get the parameters of the membership function
  vector<double> getParams() const { return params; }

  // Method to get a string that describes the type of membership function
  string getMFTypeString() const
  {
    switch (type)
    {
    case TRIANG:
      return "Triangular";
    case TRAP:
      return "Trapezoidal";
    case SAT:
      return "Saturation";
    case GAUSS:
      return "Gaussian";
    default:
      return "Unknown";
    }
  }

  // Method to evaluate the membership degree of an input value x
  // Depending on the type of membership function, the corresponding function is called
  // To calculate the membership degree
  double eval(double x) const override
  {
    double res = 0;

    switch (type)
    {
    case TRIANG:
      if (params.size() == 3)
        res = triangmf(params[0], params[1], params[2], x);
      break;
    case TRAP:
      if (params.size() == 4)
        res = trapmf(params[0], params[1], params[2], params[3], x);
      break;
    case SAT:
      if (params.size() == 2)
        res = satmf(params[0], params[1], x);
      break;
    case GAUSS:
      if (params.size() == 2)
        res = gaussianmf(params[0], params[1], x);
      break;
    default:
      cout << "No adequate MF" << endl;
      break;
    }

    return res; // Return the calculated membership degree
  }

  // Method to get the calculated membership values
  map<string, double> getMembershipValues() const { return membershipValues; }

  // Method to calculate and store the membership value for a given input value x
  void fuzzify(double x)
  {
    // Calculate the membership degree and store it in the map
    double value = eval(x);
    membershipValues[name] = value;
  }
};

// Class representing an output fuzzy set
// Inherits from the FuzzySet class
class OutputFuzzySet : public FuzzySet
{
public:
  OutputFuzzySet(string n)
      : FuzzySet(n)
  {
  } // Constructor that initializes the name of the fuzzy set

  // Method to get the parameters of the membership function
  vector<double> getParams() const { return params; }

  // Method to get a string that describes the type of membership function
  string getMFTypeString() const
  {
    switch (type)
    {
    case TRIANG:
      return "Triangular";
    case TRAP:
      return "Trapezoidal";
    case SAT:
      return "Saturation";
    case GAUSS:
      return "Gaussian";
    default:
      return "Unknown";
    }
  }

  // Method to evaluate the membership degree of an input value x
  double eval(double x) const override
  {
    // Implement if necessary
    return 0.0;
  }
};

// Class to handle fuzzy rules
class Rules
{
private:
  vector<string> rules; // Vector to store the rules

public:
  // Method to add a rule to the rule set
  void addRule(string r)
  {
    // No validation of rule existence is performed
    rules.push_back(r);
  }

  // Method to print the stored rules
  void printRules() const
  {
    std::cout << "\nRead Rules: " << endl;
    for (const auto &rule : rules)
    {
      std::cout << rule << std::endl;
    }
  }

  // Method to perform Mamdani inference
  // Takes a map containing the input membership values as an argument
  // Returns a map containing the output membership values
  // Maximum membership is used
  map<string, double> inferMamdani(map<string, double> inputMembershipValues)
  {
    multimap<string, double> outputMembershipValues;

    // Iterate over all stored rules
    for (int i = 0, j = 3; i < rules.size(); i++)
    {
      j = 3;
      string rule = rules[i];
      string fuzzySetName = "";

      cout << "IF" << endl;

      bool andFlag = false, orFlag = false;
      double accum = 0;

      // Analyze the rule to evaluate fuzzy inference
      while (fuzzySetName != "THEN" && fuzzySetName != "then")
      {
        fuzzySetName = "";
        while (rule[j] != ' ')
        {
          fuzzySetName += rule[j++];
        }

        cout << "fuzzySetName - " << fuzzySetName << endl;
        // Use AND and OR operations to combine the input membership values according to the rules
        if (fuzzySetName == "AND" || fuzzySetName == "and")
          andFlag = true;
        else if (fuzzySetName == "OR" || fuzzySetName == "or")
          orFlag = true;
        else
        {
          if (inputMembershipValues.find(fuzzySetName) !=
              inputMembershipValues.end())
          {
            double currentValue = inputMembershipValues[fuzzySetName];

            if (andFlag)
            {
              andFlag = false;
              accum = fAnd(currentValue, accum); // Calculate the AND operation
            }
            else if (orFlag)
            {
              orFlag = false;
              accum = fOr(currentValue, accum); // Calculate the OR operation
            }
            else
            {
              accum = currentValue; // First value of the rule
            }
          }
        }

        j++;
      }

      fuzzySetName = "";

      // Get the name of the output fuzzy set
      while (j < rule.length())
      {
        fuzzySetName += rule[j];
        j++;
      }

      cout << "fuzzySetName - " << fuzzySetName << endl;

      // Insert the result into the multimap storing the output values
      outputMembershipValues.insert(pair<string, double>(fuzzySetName, accum));
    }

    // Calculate the maximum of the output membership values for each output fuzzy set
    map<string, double> output;
    string currentSet = "";
    double maximum = 0;

    for (multimap<string, double>::iterator itr =
             outputMembershipValues.begin();
         itr != outputMembershipValues.end(); ++itr)
    {
      if ((*itr).first != currentSet)
      {
        maximum = 0;
        currentSet = (*itr).first;
      }

      maximum = max(maximum, (*itr).second);
      output[currentSet] = maximum;
    }

    // Add the maximum of the last output fuzzy set
    output[currentSet] = maximum;

    return output; // Return the output membership values
  }
};

// Function to read the rules from a file
// Adds them to the Rules object
// Takes the filename and the Rules object as arguments
void readRulesFromFile(const std::string &filename, Rules &rules)
{
  // Open the file in read mode
  std::ifstream file(filename);
  // String used to store each line of the file
  std::string rule;

  // Check if the file was opened successfully
  if (file.is_open())
  {
    // Read each line of the file and add it to the rule variable
    while (std::getline(file, rule))
    {
      rules.addRule(rule); // Add the rule to the Rules object
    }
    // Close the file
    file.close();
  }
  else
  {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
  }
}

// Function to read the fuzzy sets from a file
// Initializes them in vectors of fuzzy sets
// Takes the filename as an argument
// And the vectors of input and output fuzzy sets
void readFuzzySetsFromFile(const std::string &filename,
                           std::vector<InputFuzzySet> &inputSets,
                           std::vector<OutputFuzzySet> &outputSets)
{
  // Open the file in read mode
  std::ifstream file(filename);
  std::string line;

  // Read each line of the file
  while (std::getline(file, line))
  {
    // Create an input stream from the read line for processing
    std::istringstream iss(line);
    // Declare variables to store the values
    std::string setName, mfTypeStr;
    double param1, param2, param3, param4;
    vector<double> params;

    // Read the name of the fuzzy set, the type of membership function
    // And the first parameter from the line
    if (iss >> setName >> mfTypeStr >> param1)
    {
      // Initialize the number of parameters of the membership function
      int numParams = 0;
      // Determine the number of parameters of the membership function
      if (mfTypeStr == "TRIANG")
        numParams = 3;
      else if (mfTypeStr == "TRAP")
        numParams = 4;
      else if (mfTypeStr == "SAT")
        numParams = 2;
      else if (mfTypeStr == "GAUSS")
        numParams = 2;

      // Switch to process different numbers of parameters
      switch (numParams)
      {
      case 3:
        if (iss >> param2 >> param3)
        {
          params.push_back(param1);
          params.push_back(param2);
          params.push_back(param3);
        }
        else
        {
          break;
        }
        break;
      case 4:
        if (iss >> param2 >> param3 >> param4)
        {
          params.push_back(param1);
          params.push_back(param2);
          params.push_back(param3);
          params.push_back(param4);
        }
        else
        {
          break;
        }
        break;
      case 2:
        if (iss >> param2)
        {
          params.push_back(param1);
          params.push_back(param2);
          break;
        }
        break;
      default:
        break;
      }

      // Convert the membership function type string into the enumerated value
      MFType mfType;
      if (mfTypeStr == "TRIANG")
        mfType = TRIANG;
      else if (mfTypeStr == "TRAP")
        mfType = TRAP;
      else if (mfTypeStr == "SAT")
        mfType = SAT;
      else if (mfTypeStr == "GAUSS")
        mfType = GAUSS;

      // Check if the fuzzy set name contains "Tip"
      // To determine if it is an input or output set
      if (setName.find("Tip") != std::string::npos)
      {
        // Create a new output set and add it to the vector
        OutputFuzzySet outputSet(setName);
        outputSet.setMF(mfType, params);
        outputSets.push_back(outputSet);
      }
      else // Input fuzzy set
      {
        // Create a new input set and add it to the vector
        InputFuzzySet inputSet(setName);
        inputSet.setMF(mfType, params);
        inputSets.push_back(inputSet);
      }
    }
  }
}
int main()
{
  // Crisp values for service and food
  double crispInputService = 40;
  double crispInputFood = 60;

  // Vectors to store input and output fuzzy sets
  std::vector<InputFuzzySet> inputSets;
  std::vector<OutputFuzzySet> outputSets;

  // Filename that contains the definition of the fuzzy sets
  // std::string filename = "fuzzy_variables.txt";
  std::string filename = "variables.txt";

  // Read the fuzzy sets from the file and store them in the corresponding vectors
  readFuzzySetsFromFile(filename, inputSets, outputSets);

  // Print information about the input fuzzy sets
  std::cout << "Input fuzzy sets: " << std::endl;
  for (const auto &inputSet : inputSets)
  {
    std::cout << "[input]Name: " << inputSet.getName() << endl;
  }

  // Print information about the output fuzzy sets
  std::cout << "\nOutput fuzzy sets: " << std::endl;
  for (const auto &outputSet : outputSets)
  {
    std::cout << "[output]Name: " << outputSet.getName() << endl;
  }

  // Fuzzify the crisp input values for each input set
  // and store the resulting values in the map
  map<string, double> inputMembershipValues;

  // Fuzzification of crisp input values for each input fuzzy set
  for (auto &inputSet : inputSets)
  {
    // Fuzzify the service crisp value if the set name contains "Service"
    if ((inputSet.getName().find("Service") != string::npos) ||
        inputSet.getName().find("waiting_time") != string::npos)
    {
      inputSet.fuzzify(crispInputService);
    }
    // Fuzzify the food crisp value if the set name contains "Food"
    else if ((inputSet.getName().find("Food") != string::npos) ||
             (inputSet.getName().find("price") != string::npos))
    {
      inputSet.fuzzify(crispInputFood);
    }
    // Get the resulting fuzzy membership values and add them to the map
    // of membership values
    auto membershipValues = inputSet.getMembershipValues();
    inputMembershipValues.insert(membershipValues.begin(),
                                 membershipValues.end());
  }

  // Print the resulting fuzzy membership values for service and food
  cout << "\nFuzzy membership values for Service and Food: " << endl;
  for (const auto &pair : inputMembershipValues)
  {
    cout << pair.first << " -> " << pair.second << endl;
  }

  // Create an object to store the rules and read the rules from a file
  Rules rulesTipping;
  // std::string filename2 = "fuzzy_rules.txt";
  std::string filename2 = "rules.txt";
  readRulesFromFile(filename2, rulesTipping);

  // Print the loaded rules
  rulesTipping.printRules();
  cout << "\nRules added for tipping based on service and food quality\n"
       << endl;

  // Infer the output values using the rules and input fuzzy membership values
  // Store the inferred output values in a map
  map<string, double> outputValuesTipping = rulesTipping.inferMamdani(inputMembershipValues);

  // Print the inferred output values
  cout << "\nTipping inference completed. Displaying output values:" << endl;
  for (const auto &output : outputValuesTipping)
  {
    cout << output.first << ": " << output.second << endl;
  }

  cout << "\nFuzzy logic system processed all inputs successfully." << endl;

  return 0;
}
