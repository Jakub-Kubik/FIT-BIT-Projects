"""costumerental URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.urls import path
from django.conf.urls import include
from pages.views import base_view, costume_view, accessory_view
from accounts.views import login_view, logout_view, customer_signup_view, \
        manage_customers_view, manage_employees_view, employee_signup_view, \
        profile_view, change_password_view
from products.views import manage_costumes_view, manage_accessories_view, \
        costume_add_view, accessory_add_view

urlpatterns = [
    path('', base_view, name='home'),
    path('profile/', profile_view, name='profile'),
    path('profile/change_password/', change_password_view, name='change_password'),

    path('costumes/', costume_view, name='costumes'),
    path('accessories/', accessory_view, name='accessories'),

    path('accounts/login/', login_view, name='login'),
    path('accounts/signup/', customer_signup_view, name='customer_signup'),
    path('accounts/logout/', logout_view, name='logout'),
   
    path('manage_costumes/', manage_costumes_view, name='manage_costumes'),
    path('manage_costumes/add_costume/', costume_add_view, name='add_costume'),
    path('manage_accessories/', manage_accessories_view, name='manage_accessories'),
    path('manage_accessories/add_accessory/', accessory_add_view, name='add_accessory'),
    path('manage_customers/', manage_customers_view, name='manage_customers'),
    path('manage_employees/', manage_employees_view, name='manage_employees'),
    path('manage_employees/employee_signup/', employee_signup_view, name='employee_signup'),
    
    # path('admin/', admin.site.urls),
]

urlpatterns += [
    path('accounts/', include('django.contrib.auth.urls')),
]
