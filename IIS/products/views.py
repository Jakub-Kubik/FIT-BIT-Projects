from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.db import transaction
from django.contrib.auth.decorators import user_passes_test, login_required
from .forms import ProductAddForm, CostumeAddForm, AccessoryAddForm
from django.contrib.auth import get_user_model

from django.contrib import messages
from products.models import Costumes
from products.models import Accessory
from products.models import Products

User = get_user_model()


# Print or delete products #################################################################
@login_required
@user_passes_test(lambda u: u.is_superuser or u.is_employee, login_url='logout')
def manage_costumes_view(request):
    # delete costume
    if request.method == "POST" and "delete_list" in request.POST:
        products_to_delete = request.POST.getlist('delete_cost')
        if products_to_delete:
            for product in products_to_delete:
                Products.objects.filter(product_id=int(product)).delete()
            messages.success(request, "Kostým bol odstránený")
        else:
            messages.info(request, "Kostým sa nepodarilo odstáminť")

    # print costumes
    if request.user.is_superuser:
        products = Products.objects.filter(is_costume=True)
    else:
        products = Products.objects.filter(is_costume=True).filter(manager=request.user.username)
    context = {'products': products}

    return render(request, 'adm_manage_cost.html', context)


@login_required
@user_passes_test(lambda u: u.is_superuser or u.is_employee, login_url='logout')
def manage_accessories_view(request):
    # delete access  
    if request.method == "POST" and "delete_list" in request.POST:
        products_to_delete = request.POST.getlist('delete_access')
        if products_to_delete:
            for product in products_to_delete:
                Products.objects.filter(product_id=int(product)).delete()
            messages.success(request, "Doplnok bol odstránený")
        else:
            messages.info(request, "Doplnok sa nepodarilo odstáminť")

    # print access
    if request.user.is_superuser:
        products = Products.objects.filter(is_costume=False)
    else:
        products = Products.objects.filter(is_costume=False).filter(manager=request.user.username)
    context = {'products': products}

    return render(request, 'adm_manage_access.html', context)


# Add products ################################################################
@login_required
@user_passes_test(lambda u: u.is_superuser or u.is_employee, login_url='logout')
@transaction.atomic
def costume_add_view(request):
    # add costume
    product_form = ProductAddForm(request.POST or None)
    costumes_form = CostumeAddForm(request.POST or None)

    if all([product_form.is_valid(), costumes_form.is_valid()]):
        product = product_form.save()
        product.is_costume = True
        product.manager = User.objects.get(username=request.user.username)

        product.save()

        print("fdsfgsd")
        costume = Costumes.objects.create(product=product)
        costume.name = costumes_form.cleaned_data.get('name')
        costume.description = costumes_form.cleaned_data.get('description')
        costume.manufacturer = costumes_form.cleaned_data.get('manufacturer')
        costume.fabric = costumes_form.cleaned_data.get('fabric')
        costume.color = costumes_form.cleaned_data.get('color')
        costume.size = costumes_form.cleaned_data.get('size')

        costume.amount = costumes_form.cleaned_data.get('amount')
        costume.price = costumes_form.cleaned_data.get('price')

        costume.save()

        messages.success(request, "Doplnok bol úspešne pridaný")
        return HttpResponseRedirect('/manage_costumes/')

    return render(request, "form_add_cost.html", {
        'forms': {
            'product_form': product_form,
            'costume_form': costumes_form,
        }
    }
                  )


@login_required
@user_passes_test(lambda u: u.is_superuser or u.is_employee, login_url='logout')
@transaction.atomic
def accessory_add_view(request):
    # add accessory
    product_form = ProductAddForm(request.POST or None)
    accessory_form = AccessoryAddForm(request.POST or None)

    if all([product_form.is_valid(), accessory_form.is_valid()]):
        product = product_form.save()
        product.is_costume = False
        product.manager = User.objects.get(username=request.user.username)

        product.save()

        accessory = Accessory.objects.create(product=product)
        accessory.name = accessory_form.cleaned_data.get('name')
        accessory.description = accessory_form.cleaned_data.get('description')
        accessory.amount = accessory_form.cleaned_data.get('amount')
        accessory.price = accessory_form.cleaned_data.get('price')

        accessory.save()

        messages.success(request, "Doplnok bol úspešne pridaný")
        return HttpResponseRedirect('/manage_accessories/')

    return render(request, "form_add_access.html", {
        'forms': {
            'product_form': product_form,
            'accessory_form': accessory_form,
        }
    }
                  )
