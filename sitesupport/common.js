/*
 * Javacript for the site
 */

/* jshint browser: true */
/* jshint -W097 */
/* exported escapeHtml, showSpinner, hideSpinner, urlsafeB64Encode,
            urlsafeB64Decode */

'use strict';

function escapeHtml(text) {
  var map = {
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    '"': '&quot;',
    '\'': '&#039;'
  };

  return text.replace(/[&<>"']/g, function(m) { return map[m]; });
}

function showSpinner() {
  var dlg = document.getElementById('refresh_spinner');
  if (!dlg) {
    dlg = document.createElement('DIV');
    dlg.classList.add('UI-modal');
    dlg.id = 'refresh_spinner';
    var content = document.createElement('DIV');
    dlg.appendChild(content);
    content.classList.add('UI-spinner-div');
    content.style.width = '90px';
    content.style.backgroundColor = 'orange';
    var span = document.createElement('SPAN');
    span.innerHTML = '<i class="fas fa-sync UI-spin"></i>';
    content.appendChild(span);
    document.body.appendChild(dlg);
  }
  dlg.style.display = 'block';
}

function hideSpinner() {
  document.getElementById('refresh_spinner').style.display = 'none';
}

function urlsafeB64Encode(data) {
  return btoa(data).replace(/\+/g, '-').replace(/\//g, '_');
}

function urlsafeB64Decode(data) {
  return atob(data.replace(/-/g, '+').replace(/_/g, '/'));
}
