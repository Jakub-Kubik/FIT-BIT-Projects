from django import forms
from django.contrib.auth import get_user_model
from django.contrib.auth.forms import UserCreationForm, AuthenticationForm, \
    PasswordChangeForm
from django.core.validators import RegexValidator
from .models import Employee
import datetime

User = get_user_model()


class UserLoginForm(AuthenticationForm):
    username = forms.CharField(
        label="",
        max_length=64,
        required=True,
        widget=forms.TextInput(attrs={
            "placeholder": "Prihlasovacie meno",
            "class": "form-control"
        })
    )
    password = forms.CharField(
        label="",
        max_length=64,
        required=True,
        widget=forms.PasswordInput(attrs={
            "placeholder": "Heslo",
            "class": "form-control"
        })
    )


class CustomerSignupForm(UserCreationForm):
    username = forms.CharField(label='Užívateľské meno',
                               widget=forms.TextInput(
                                   attrs={
                                       'class': 'form-control',
                                       'placeholder': 'Užívateľské meno',
                                   }
                               ),
                               validators=[
                                   RegexValidator(
                                       regex='^([a-z]|[0-9]|_){8}$',
                                       message="Login môže obsahovať maximálne 8 znakov ([a-z]|[0-9]|_).",
                                   )
                               ]
                               )

    first_name = forms.CharField(required=False,
                                 label='Krstné meno',
                                 widget=forms.TextInput(
                                     attrs={
                                         'class': 'form-control',
                                         'placeholder': 'Krstné meno',
                                     }
                                 )
                                 )
    last_name = forms.CharField(required=False,
                                label='Priezvisko',
                                widget=forms.TextInput(
                                    attrs={
                                        'class': 'form-control',
                                        'placeholder': 'Priezvisko',
                                    }
                                )
                                )
    email = forms.EmailField(required=False,
                             label='Email',
                             widget=forms.TextInput(
                                 attrs={
                                     'class': 'form-control',
                                     'placeholder': 'Email',
                                 }
                             )
                             )
    password1 = forms.CharField(label='Heslo',
                                widget=forms.PasswordInput(
                                    attrs={
                                        'class': 'form-control',
                                        'placeholder': 'Heslo',
                                    }
                                )
                                )
    password2 = forms.CharField(label='Potvrdenie hesla',
                                widget=forms.PasswordInput(
                                    attrs={
                                        'class': 'form-control',
                                        'placeholder': 'Potvrď heslo',
                                    }
                                )
                                )

    class Meta:
        model = User
        fields = ('username', 'email', 'first_name', 'last_name')
        help_texts = {
            'username': None,
        }


class EmployeeSignupForm(forms.ModelForm):
    birth_date = forms.DateField(
        widget=forms.DateInput(attrs={
            'class': 'form-control',
        }
        ),
        initial=datetime.date.today()
    )
    position = forms.ChoiceField(
        widget=forms.Select(attrs={
            'class': 'form-control',
        }
        ),
        choices=[('Žehlič', 'Žehlič'), ('Pokladník', 'Pokladník'), ('Skladník', 'Skladník')]
    )

    class Meta:
        model = Employee
        fields = ('birth_date', 'position')


class CustomChangePasswordForm(PasswordChangeForm):
    old_password = forms.CharField(
        widget=forms.PasswordInput(attrs={
            'class': 'form-control',
            'placeholder': 'Staré heslo',
        }
        ),
        label="Staré heslo",
    )
    new_password1 = forms.CharField(
        widget=forms.PasswordInput(attrs={
            'class': 'form-control',
            'placeholder': 'Nové heslo',
        }
        ),
        label="Nové heslo"
    )
    new_password2 = forms.CharField(
        widget=forms.PasswordInput(attrs={
            'class': 'form-control',
            'placeholder': 'Nové heslo',
        }
        ),
        label="Nové heslo (ešte raz)"
    )
