from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.contrib.auth import login, logout, get_user_model, update_session_auth_hash
from .forms import UserLoginForm, CustomerSignupForm, EmployeeSignupForm, \
                    CustomChangePasswordForm

from django.contrib.auth.decorators import user_passes_test, login_required
from django.contrib import messages
from .models import Employee
from django.db import transaction
# Create your views here.

User = get_user_model()


def login_view(request):
    form = UserLoginForm(data=request.POST or None)
    if form.is_valid():
        user = form.get_user()
        login(request, user)

        if 'next' in request.POST:
            return HttpResponseRedirect(request.POST.get('next'))
        return HttpResponseRedirect('/profile/')

    return render(request, "form_login.html", {'form': form})


def customer_signup_view(request):
    form = CustomerSignupForm(request.POST or None)
    if form.is_valid():
        user = form.save()
        user.is_customer = True
        user.save()
        login(request, user)
        messages.success(request, "Registrácia prebehla úspešne")
        return HttpResponseRedirect('/profile/')
    return render(request, "form_reg.html", {'form': form})


@login_required
@user_passes_test(lambda u: u.is_superuser, login_url='logout')
@transaction.atomic
def employee_signup_view(request):
    basic_form = CustomerSignupForm(request.POST or None)
    employee_form = EmployeeSignupForm(request.POST or None)

    if all([basic_form.is_valid(), employee_form.is_valid()]):
        user = basic_form.save()
        user.is_employee = True
        user.save() 

        employee = Employee.objects.create(user=user)
        employee.birth_date = employee_form.cleaned_data.get('birth_date')
        employee.position = employee_form.cleaned_data.get('position')
        employee.save()

        messages.success(request, "Zamestnanec bol úspešne zaregistrovaný")
        return HttpResponseRedirect('/manage_employees/')

    return render(request, "form_add_emp.html", {
                'forms': {
                    'basic_form': basic_form,
                    'employee_form': employee_form, 
                }
            }
        )


def logout_view(request):
    logout(request)
    return HttpResponseRedirect('/')


@login_required
@user_passes_test(lambda u: u.is_superuser or u.is_employee, login_url='logout')
def manage_customers_view(request):
    customers = User.objects.filter(is_customer=True)
    context = {
        'customers': customers,
    }

    if request.method == "POST" and "delete_list" in request.POST:
        customers_to_delete = request.POST.getlist('delete_cust')
        if customers_to_delete:
            for customer in customers_to_delete:
                User.objects.filter(username=customer).delete()
            messages.success(request, "Zákazník bol odstránený")
        else:
            messages.info(request, "Zákazníka sa nepodarilo odstrániť")

    return render(request, "adm_manage_cust.html", context)


@login_required
@user_passes_test(lambda u: u.is_superuser, login_url='logout')
def manage_employees_view(request):
    employees = User.objects.filter(is_employee=True)
    context = {
        'employees': employees,
    }
    if request.method == "POST" and "delete_list" in request.POST:
        employees_to_delete = request.POST.getlist('delete_emp')
        if employees_to_delete:
            for employee in employees_to_delete:
                User.objects.filter(username=employee).delete()
            messages.success(request, "Zamestnanec bol odstránený")
        else:
            messages.info(request, "Zákazníka sa nepodarilo odstrániť")

    return render(request, "adm_manage_emp.html", context)


@login_required
def profile_view(request):
    return render(request, "cust_logged.html", {})


@login_required
def change_password_view(request):
    if request.method == "POST":
        form = CustomChangePasswordForm(request.user, request.POST)
        if form.is_valid():
            user = form.save()
            update_session_auth_hash(request, user)
            messages.success(request, "Heslo bolo úspešne zmenené")
            return HttpResponseRedirect('/profile/')
        else:
            messages.error(request, "Heslo sa nepodarilo zmeniť")
    else:
        form = CustomChangePasswordForm(request.user)

    return render(request, "change_password.html", {'form': form})

