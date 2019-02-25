from django import forms
import datetime
from .models import Products
from .models import Accessory
from .models import Costumes


class ProductAddForm(forms.ModelForm):
    deprecation = forms.CharField(
        max_length=20,
        required=True,
        label="Opotrebenie",
        widget=forms.TextInput(attrs={
            "placeholder": "Opotrebenie",
            "class": "form-control"
        }
        )
    )

    assembly_data = forms.DateField(
        label="Dátum výroby",
        widget=forms.DateInput(attrs={
            "class": "form-control",
        }
        ),
        initial=datetime.date.today()
    )

    class Meta:
        model = Products
        fields = ('deprecation', 'assembly_data')


class CostumeAddForm(forms.ModelForm):
    name = forms.CharField(
        max_length=32,
        required=True,
        label="Názov",
        widget=forms.TextInput(attrs={
            "placeholder": "Názov",
            "class": "form-control"
        })
    )

    description = forms.CharField(
        max_length=128,
        required=False,
        label="Popis",
        widget=forms.DateInput(attrs={
            "placeholder": "Popis",
            "class": "form-control"
        })
    )

    manufacturer = forms.CharField(
        max_length=32,
        required=False,
        label="Výrobca",
        widget=forms.DateInput(attrs={
            "placeholder": "Výrobca",
            "class": "form-control"
        })
    )

    fabric = forms.CharField(
        max_length=32,
        required=False,
        label="Látka",
        widget=forms.DateInput(attrs={
            "placeholder": "Látka",
            "class": "form-control"
        })
    )

    color = forms.CharField(
        max_length=16,
        required=True,
        label="Výrobca",
        widget=forms.DateInput(attrs={
            "placeholder": "Výrobca",
            "class": "form-control"
        })
    )

    size = forms.ChoiceField(
        required=True,
        label="Veľkosť",
        widget=forms.Select(attrs={
            "class": "form-control"
        }),
        choices=[('XS', 'XS'), ('S', 'S'), ('M', 'M'), ('L', 'L'), ('XL', 'XL'), ('XXL', 'XXL')]
    )

    amount = forms.IntegerField(
        required=True,
        label="Množstvo",
        widget=forms.NumberInput(attrs={
            "placeholder": "Množstvo",
            "class": "form-control"
        }
        ),

    )
    price = forms.FloatField(
        required=True,
        label="Cena",
        widget=forms.TextInput(attrs={
            "placeholder": "Cena",
            "class": "form-control"
        })
    )

    class Meta:
        model = Costumes
        fields = ('name', 'description', 'manufacturer', 'fabric', 'color', 'size', 'amount', 'price')


class AccessoryAddForm(forms.ModelForm):
    name = forms.CharField(
        max_length=32,
        required=True,
        label="Názov",
        widget=forms.TextInput(attrs={
            "placeholder": "Názov",
            "class": "form-control"
        })
    )

    description = forms.CharField(
        max_length=128,
        required=False,
        label="Popis",
        widget=forms.DateInput(attrs={
            "placeholder": "Popis",
            "class": "form-control"
        })
    )

    amount = forms.IntegerField(
        required=True,
        label="Množstvo",
        widget=forms.NumberInput(attrs={
            "placeholder": "Množstvo",
            "class": "form-control"
        }
        ),

    )
    price = forms.FloatField(
        required=True,
        label="Cena",
        widget=forms.TextInput(attrs={
            "placeholder": "Cena",
            "class": "form-control"
        })
    )

    class Meta:
        model = Accessory
        fields = ('name', 'description', 'amount', 'price')
