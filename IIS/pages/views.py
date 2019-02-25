from django.shortcuts import render
from products.models import Costumes
from products.models import Accessory

# Create your views here.


def base_view(request):
    return render(request, 'main_content.html', {})


def costume_view(request):
    costumes = Costumes.objects.all()
    context = {'costumes': costumes}
    return render(request, 'costume.html', context)


def accessory_view(request):
    accessory = Accessory.objects.all()
    context = {'accessory': accessory}
    return render(request, 'accessory.html', context)
