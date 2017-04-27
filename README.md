# Mail-Merge

*CME 2002 Data Organization Management*


# Scenario and Implementation Requirements

Program will take the template and variable files as input, replace each placeholder with the corresponding value from the variable file for each student and output each to a file named as the value of the id and with a fixed extension “txt”. (Example: 2008900.txt) So for sure variables file will have an attribute or element named “id” for each object, and you do not need to check the existence of it. Examples input files are given below.

# ---------------------------------------
Example template file: template.xml

<mail>

Dear Student <name/> <surname/>,
This letter is an example to inform you about the content of template file. Your student
should be <id/> and your GPA is to be <gpa/>. Thanks for reading.
Your address is: <address/>
<postcode/> <city/>
</mail>
# ---------------------------------------
