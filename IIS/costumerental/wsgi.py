# This file contains the WSGI configuration required to serve up your
# web application at http://costrental.pythonanywhere.com/
# It works by setting the variable 'application' to a WSGI handler of some
# description.
#

# +++++++++++ GENERAL DEBUGGING TIPS +++++++++++
# getting imports and sys.path right can be fiddly!
# We've tried to collect some general tips here:
# https://www.pythonanywhere.com/wiki/DebuggingImportError


# +++++++++++ DJANGO +++++++++++
# To use your own django app use code like this:
import os
import sys
#
# assuming your django settings file is at '/home/costrental/mysite/mysite/settings.py'
# and your manage.py is is at '/home/costrental/mysite/manage.py'
from django.core.wsgi import get_wsgi_application
path = '/home/costrental/IIS'
if path not in sys.path:
    sys.path.append(path)
#
os.environ['DJANGO_SETTINGS_MODULE'] = 'costumerental.settings'

application = get_wsgi_application()
